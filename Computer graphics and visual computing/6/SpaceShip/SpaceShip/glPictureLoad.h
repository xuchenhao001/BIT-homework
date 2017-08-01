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
	int m_nColor;//һ����ɫ��ռ�ֽڸ���.
	unsigned char *m_pData;
	int m_nCompressedSize;	//ѹ����Ĵ�С
	int m_iCompressedFormat;//ѹ����ʽ
	int m_nLevel;			//����
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
	//sFilePathName�ļ�����
	//transcolor͸��ɫ������ɫ��ȫ͸����
	//transdegree͸���ȱ�ʾ��͸��ɫ���ֵ�͸��ȣ�
	//fAlpha����Ҫ����͹��ջ�ϣ���ʾ����ռ�����ɫ�еİٷֱ�
	//diffuse����Ҫ����͹��ջ�ϣ���ʾ�����ʹ�õĹ�����ɫ�������ɫһ�������ڲ����е�����⡣
	void * GetData(CString sFilePathName,int transcolor,unsigned char transdegree,float fAlpha,float *diffuse);
private:
	unsigned char m_cTransDegree;//	͸���ȡ�һ�������͸���̶ȡ�
	VOID Init();
	BOOL Load(CString sFilePathName);//���ļ���ȡͼ��
	BOOL Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType);//����Դ��ȡͼ��
	void Render(CDC* pDC, LPRECT pDrawRect/*Ŀ����Σ���λ���߼����굥λ*/
		,LPRECT pSrcRect=NULL/*��Դ���Σ���λ��0.01����,���Ϊ�գ�����������ͼ��Ŀ�����*/
		,LPCRECT prcWBounds=NULL/*ͼԪ�ļ�ר�ã��󶨾���*/
		);//�ڸ�����DC�ϻ�ͼ��
	BOOL LoadPictureData(BYTE* pBuffer, int nSize);//���ڴ��ȡͼ��
	BOOL SaveAsBitmap(CString sFilePathName);//д�뵽BMP�ļ�
	void UnloadPicture();//�ͷ�ͼ������ͬCPictureHolder::~CPictureHolder()
	LONG get_Height(); // ��0.01����Ϊ��λ��ͼ��߶�
	LONG get_Width(); // ��0.01����Ϊ��λ��ͼ����
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
	int m_iTransparence;//͸��ɫ
	float m_fAlpha;//�����alphaֵ
	float m_diffuse[3];//��Ҫ������ϳɵ���ɫֵ
};

#endif