#if !defined(AFX_CglPictureLoad_H)
#define AFX_CglPictureLoad_H

#pragma once
#include "gl/gl.h"
#define  BYTE unsigned char
#define  WORD unsigned short
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)
#include <afxctl.h>
struct CglImageData
{
	int m_Width;
	int m_Height;
	int m_nColor;//一个颜色所占字节个数.
	unsigned char *m_pData;
	int m_nCompressedSize;	//压缩后的大小
	int m_iCompressedFormat;//压缩格式
	int m_nLevel;			//级别
	CglImageData()
	{
		m_nLevel=0;
		m_nColor=0;
		m_Width=0;
		m_Height=0;
		m_pData=NULL;
		m_nCompressedSize=0;
	}
	~CglImageData()
	{
		if(m_pData)
			delete m_pData;
		m_pData=NULL;
		m_nCompressedSize=0;
	}
	void SaveData(FILE *fp)
	{
		fwrite(&m_Width,4,1,fp);
		fwrite(&m_Height,4,1,fp);
		fwrite(&m_nColor,4,1,fp);
		fwrite(&m_nCompressedSize,4,1,fp);
		fwrite(&m_iCompressedFormat,4,1,fp);
		fwrite(&m_nLevel,4,1,fp);
		if(m_nCompressedSize>0)
		{
			fwrite(&m_nLevel,4,1,fp);
			fwrite(m_pData,1,m_nCompressedSize,fp);
		}
		else
			fwrite(m_pData,1,m_Width*m_Height*m_nColor,fp);
	}
	void LoadData(FILE *fp)
	{
		fread(&m_Width,4,1,fp);
		fread(&m_Height,4,1,fp);
		fread(&m_nColor,4,1,fp);
		fread(&m_nCompressedSize,4,1,fp);
		fread(&m_iCompressedFormat,4,1,fp);
		fread(&m_nLevel,4,1,fp);
		if(m_pData)
			delete m_pData;
		if(m_nCompressedSize>0)
		{
			fread(&m_nLevel,4,1,fp);
			m_pData = new unsigned char [m_nCompressedSize];
			fread(m_pData,1,m_nCompressedSize,fp);
		}
		else
		{
			m_pData = new unsigned char [m_Width*m_Height*m_nColor];
			fread(m_pData,1,m_Width*m_Height*m_nColor,fp);
		}
	}
};
class AFX_CLASS_EXPORT CglPictureLoad: public CPictureHolder  
{
public:
	CglPictureLoad(void);
public:
	~CglPictureLoad(void);
	//sFilePathName文件名，
	//transcolor透明色：此颜色完全透明，
	//transdegree透明度表示非透明色部分的透光度，
	//fAlpha，需要纹理和光照混合，表示纹理占混合颜色中的百分比
	//diffuse，需要纹理和光照混合，表示混合中使用的光照颜色。这个颜色一般来自于材质中的漫射光。
	void * GetData(CString sFilePathName,int transcolor,unsigned char transdegree,float fAlpha,float *diffuse);
private:
	unsigned char m_cTransDegree;//	透明度。一个纹理的透明程度。
	VOID Init();
	BOOL Load(CString sFilePathName);//从文件读取图像
	BOOL Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType);//从资源读取图像
	void Render(CDC* pDC, LPRECT pDrawRect/*目标矩形，单位是逻辑坐标单位*/
		,LPRECT pSrcRect=NULL/*来源矩形，单位是0.01毫米,如果为空，则拉伸整个图像到目标矩形*/
		,LPCRECT prcWBounds=NULL/*图元文件专用，绑定矩形*/
		);//在给定的DC上画图，
	BOOL LoadPictureData(BYTE* pBuffer, int nSize);//从内存读取图像
	BOOL SaveAsBitmap(CString sFilePathName);//写入到BMP文件
	void UnloadPicture();//释放图像，作用同CPictureHolder::~CPictureHolder()
	LONG get_Height(); // 以0.01毫米为单位的图像高度
	LONG get_Width(); // 以0.01毫米为单位的图像宽度
	INT GetWidth();
	INT GetHeight();
	VOID CalculateWidthHeight();
	int m_Width,m_Height;
	float m_PixelToMillimeterY;
	float m_PixelToMillimeterX;
	HDC m_hdc;
	bool m_bInit;
	void * GetBMPData(CString sFilePathName);
	void * GetTGAData(CString sFilePathName);
	void * GetOtherData(CString sFilePathName);
	int m_iTransparence;//透明色
	float m_fAlpha;//纹理的alpha值
	float m_diffuse[3];//需要与纹理合成的颜色值
};

#endif