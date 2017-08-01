
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
	float m_ambient[4];//����
	float m_diffuse[4];//ɢ��
	float m_specular[4];//�߹�
	float m_emission[4];//�Է���
	float m_shininess;//���
	float m_transparent;//͸����						�����ʾ�������������͸���ģ�͸����Ϊ��
	
	CString m_name;//���ʵ�����
	UINT m_uType;//��������
	void RemoveAll();
	UINT LoadTexture();//װ�������ļ�
	unsigned int TextureMap(CglImageData * TextureImage);
	static CglPictureLoad m_pic;
	int m_transcolor;//͸��ɫ	��Ϊ-1��ʾ������͸��	�����ʾ����ɫΪ��ȫ͸���ġ�
	CArray<CglImageData*,CglImageData*>m_CompressTextureImage;	//ѹ������������
public:
	CglImageData *m_TextureImage;
	UINT m_texid;//����id
	float m_fAlpha;//��������յĺϳɱ���
	CString m_filename;//�����ļ���
private:
	PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
	bool BuildMipMapTexture();
	bool BuildMipMapTexture(int colnum,int width,int height,unsigned char *pData);//����mipmap�����Լ����죬���Կ���
	void CompressTexture();
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(float r, float g, float b);
	void SetEmission(float r, float g, float b);
	void SetEmissionIntension(float Intension);
	void SetShininess(float s);
	void SetTransparent(float trans,int iTransColor);	//����͸����ɫ
	void SetName(CString name);
	void SetGamma(int id, float r, float g, float b);
	bool CompareName(CString name);
	void SetAmbient(float r, float g, float b);
	UINT ShareTexture(CglMaterial *mat);
	//a�Ǳ�ʾ�������͹��ղ��ʻ�ϣ�������ռ�İٷֱȡ���a=100��������ȫȡ������������Ǵ�3ds�ļ���ֱ�����������õ�
	UINT SetTexture(CString filename,int a=100);	
	UINT SetCubeTexture(CString *filename);			//ʹ����������ͼ
public:
	void SaveData(FILE *fp,int minTexid);	//�������ݱ������ļ��У�ϵͳ���á�
	void LoadData(FILE *fp,int minTexid);	//װ�ر������ļ��е��������ݣ�ϵͳ����
	//iTransColorΪ��ȫ͸������ɫ�������λΪ1ʱ��ʾû��͸����ɫ��
	//trans��ʾ������ɫ��͸���ȡ�
	//a��ʾ����Ͳ����еĹ�����ɫ���ӵı�������a=1��ʾ��ȫ���ӡ�
	UINT LoadTexture(CString filename,int iTransColor=-1,float trans=1);
	//��Ⱦ�˲���
	void Render(void);
};


#endif