
#if !defined(AFX_CglMaterial_H)
#define AFX_CglMaterial_H
#pragma once
#include "gl/gl.h"
#include "glPictureLoad.h"
#include <afxtempl.h>
#include "gl/glext.h"

class AFX_CLASS_EXPORT CglMaterial
{
public:
	CglMaterial(void);
public:
	~CglMaterial(void);
	friend class Cgl3dsLoader;
	friend class CglMeshBranch;
	friend class CglMeshLeaf;
private:
	float m_ambient[4];//环境
	float m_diffuse[4];//散射
	float m_specular[4];//高光
	float m_emission[4];//自发光
	float m_shininess;//光斑
	float m_transparent;//透明度						这个表示这个纹理整个是透明的，透明度为此
	
	CString m_name;//材质的名字
	UINT m_uType;//材质类型
	void RemoveAll();
	UINT LoadTexture();//装载纹理文件
	unsigned int TextureMap(CglImageData * TextureImage);
	static CglPictureLoad m_pic;
	int m_transcolor;//透明色	当为-1表示不允许透明	这个表示此颜色为完全透明的。
	CArray<CglImageData*,CglImageData*>m_CompressTextureImage;	//压缩的纹理数据
public:
	CglImageData *m_TextureImage;
	UINT m_texid;//纹理id
	float m_fAlpha;//纹理与光照的合成比例
	CString m_filename;//纹理文件名
private:
	PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
	bool BuildMipMapTexture();
	bool BuildMipMapTexture(int colnum,int width,int height,unsigned char *pData);//构造mipmap纹理，自己构造，可以控制
	void CompressTexture();
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(float r, float g, float b);
	void SetEmission(float r, float g, float b);
	void SetEmissionIntension(float Intension);
	void SetShininess(float s);
	void SetTransparent(float trans,int iTransColor);	//设置透明颜色
	void SetName(CString name);
	void SetGamma(int id, float r, float g, float b);
	bool CompareName(CString name);
	void SetAmbient(float r, float g, float b);
	UINT ShareTexture(CglMaterial *mat);
	//a是表示如果纹理和光照材质混合，纹理所占的百分比。当a=100则纹理完全取代。这个函数是从3ds文件中直接设置纹理用的
	UINT SetTexture(CString filename,int a=100);	
	UINT SetCubeTexture(CString *filename);			//使用立方体贴图
public:
	void SaveData(FILE *fp,int minTexid);	//纹理数据保存在文件中，系统调用。
	void LoadData(FILE *fp,int minTexid);	//装载保存在文件中的纹理数据，系统调用
	//iTransColor为完全透明的颜色，当最高位为1时表示没有透明颜色。
	//trans表示其他颜色的透明度。
	//a表示纹理和材质中的光照颜色叠加的比例。当a=1表示完全叠加。
	UINT LoadTexture(CString filename,int iTransColor=-1,float trans=1);
	//渲染此材质
	void Render(void);
};


#endif