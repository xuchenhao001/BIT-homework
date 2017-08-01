#include "stdafx.h"
#include "glMaterial.h"
#include "glPictureLoad.h"
#include "math.h"
#include "gl/glext.h"
#include "gl/glut.h"
CglPictureLoad CglMaterial::m_pic;
CglMaterial::CglMaterial(void)
{
	m_uType=0;
	m_texid=0;
	int i;
	for( i=0;i<4;i++)
	{
		m_ambient[i]=1;
		m_diffuse[i]=1;
		m_specular[i]=1;
		m_emission[i]=1;
	}
	for(i=0;i<3;i++)
	{
		m_specular[i]=0;
		m_emission[i]=0;
		m_ambient[i]=0.2;
	}
	m_shininess=0;
	m_transparent=1;
	m_transcolor = -1;
	m_fAlpha=1;
	m_TextureImage=0;
	glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) wglGetProcAddress( "glGetCompressedTexImage" );
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) wglGetProcAddress( "glCompressedTexImage2D" );	
}
void CglMaterial::RemoveAll()
{
	if(m_texid)
	{
		glDeleteTextures(1,&m_texid);
	}
	if(m_TextureImage)
		delete m_TextureImage;
	m_TextureImage=0;
	
	int i,n = m_CompressTextureImage.GetSize();
	for(i=0;i<n;i++)
		delete m_CompressTextureImage[i];
	m_CompressTextureImage.RemoveAll();
}
CglMaterial::~CglMaterial(void)
{
	RemoveAll();
}
void CglMaterial::SetName(CString name)
{
	m_name=name;
}
UINT CglMaterial::LoadTexture(CString filename,int iTransColor,float trans)
{
	SetTransparent(trans,iTransColor);
	m_filename=filename;
	m_texid = LoadTexture();
	if(m_texid)
		m_uType |= 1;
	return m_texid;
}
UINT CglMaterial::SetTexture(CString filename,int a)
{
	m_fAlpha = a/100.0;
	m_filename=filename;
	m_texid = LoadTexture();
	if(m_texid)
		m_uType |= 1;
	return m_texid;
}
UINT CglMaterial::ShareTexture(CglMaterial *mat)
{
	m_fAlpha = mat->m_fAlpha;
	m_filename=mat->m_filename;
	m_texid = mat->m_texid;
	if(m_texid)
		m_uType |= 1;
	return m_texid;
}

UINT CglMaterial::SetCubeTexture(CString *filename)
{
/*	glEnable ( GL_TEXTURE_CUBE_MAP_EXT );	//������Ҫ����CUBE_MAP
	glGenTextures(1, &m_texid);				//��������
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, m_texid);	//���������
	glTexParameteri ( GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );	//�趨�����˲���ʽ
	glTexParameteri ( GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );	
	for(int i=0;i<6;i++)					//����6�������������
	{
		//���棬���ļ��ж�ȡ����
		//������width
		//����߶�height
		//��������imagebuffer,��������ǰע������ռ�
		unsigned  texture=0;
		gluBuild2DMipmaps ( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, GL_RGB8, 
				width, height, 
				GL_RGB, GL_UNSIGNED_BYTE, imagebuffer );
		delete imagebuffer;	//�������������Ѿ����Ƶ��������У����Դ�ʱ�����ͷŵ���ǰ����Ŀռ�
	}
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );	//���ñ�Ե���Ʒ�ʽ��
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );

	glDisable ( GL_TEXTURE_CUBE_MAP_EXT );	
	if(m_texid)
		m_uType |= 1;
		*/
	return m_texid;
}
void CglMaterial::SetAmbient(float r, float g, float b)
{
	m_ambient[0]=r;
	m_ambient[1]=g;
	m_ambient[2]=b;
	m_uType |= 2;
}

void CglMaterial::SetDiffuse(float r, float g, float b)
{
	m_diffuse[0]=r;
	m_diffuse[1]=g;
	m_diffuse[2]=b;
	m_uType |= 4;
}

void CglMaterial::SetSpecular(float r, float g, float b)
{
	m_specular[0]=r;
	m_specular[1]=g;
	m_specular[2]=b;
	m_uType |= 8;
}
void CglMaterial::SetEmission(float r, float g, float b)
{
	m_emission[0]=r;
	m_emission[1]=g;
	m_emission[2]=b;
	m_uType |= 16;
}
void CglMaterial::SetEmissionIntension(float Intension)
{
	if(Intension==0) return;
	m_emission[0]=Intension*m_diffuse[0];
	m_emission[1]=Intension*m_diffuse[1];
	m_emission[2]=Intension*m_diffuse[2];
	m_uType |= 16;
}
void CglMaterial::SetShininess(float s)
{
	m_shininess=s;
	m_uType |= 32;
}
void CglMaterial::SetTransparent(float trans,int iTransColor)
{	//�������������˵��ʹ����������ʱ�ƹ�������ݽ�Ҫ�������������У����Բ���Ҫ���ƹ�����Ϣ�ˣ�ֱ�ӻ�������
	m_transparent=trans;	
	m_transcolor=iTransColor;
	if(trans<1||m_transcolor!=-1)	//��͸���Ȳ���100%����ʾ͸������������͸��
	{
		m_uType |= 64;	//��ʾ����͸��
	}
}
UINT CglMaterial::LoadTexture()
{
	unsigned int texture=0;
	if(m_TextureImage)
		delete m_TextureImage;
	m_TextureImage = (CglImageData*)m_pic.GetData(m_filename,m_transcolor,m_transparent*255,m_fAlpha,m_diffuse);
	if(m_TextureImage)
	{
		texture=TextureMap(m_TextureImage);
	}
	return texture;
}
void CglMaterial::CompressTexture()
{

}
void CglMaterial::SaveData(FILE *fp,int minTexid)
{
	//���������Ϣ
/*	float m_ambient[4];//����
	float m_diffuse[4];//ɢ��
	float m_specular[4];//�߹�
	float m_emission[4];//�Է���
	float m_shininess;//���
	float m_transparent;//͸����						�����ʾ�������������͸���ģ�͸����Ϊ��
//	CString m_filename;//�����ļ���
//	CString m_name;//���ʵ�����
//	UINT m_texid;//����id
	UINT m_uType;//��������
	int m_transcolor;//͸��ɫ	��Ϊ-1��ʾ������͸��	�����ʾ����ɫΪ��ȫ͸���ġ�
	float m_fAlpha;//��������յĺϳɱ���
	CglImageData *m_TextureImage;
*/

	fwrite(m_ambient,4,4,fp);
	fwrite(m_diffuse,4,4,fp);
	fwrite(m_specular,4,4,fp);
	fwrite(m_emission,4,4,fp);

	fwrite(&m_shininess,4,1,fp);
	fwrite(&m_transparent,4,1,fp);
	fwrite(&m_uType,4,1,fp);
	fwrite(&m_transcolor,4,1,fp);
	fwrite(&m_fAlpha,4,1,fp);

	int n = m_CompressTextureImage.GetSize();
	fwrite(&n,4,1,fp);
	for(int i=0;i<n;i++)
	{
		m_CompressTextureImage[i]->SaveData(fp);
	}
	if(n==0)//��ʾ�й��������
	{
		int ntexid = 0;
		if(m_texid)
			ntexid = m_texid - minTexid;
		fwrite(&ntexid,4,1,fp);
	}
}
void CglMaterial::LoadData(FILE *fp,int minTexid)
{
	RemoveAll();
	fread(m_ambient,4,4,fp);
	fread(m_diffuse,4,4,fp);
	fread(m_specular,4,4,fp);
	fread(m_emission,4,4,fp);
	
	fread(&m_shininess,4,1,fp);
	fread(&m_transparent,4,1,fp);
	fread(&m_uType,4,1,fp);
	fread(&m_transcolor,4,1,fp);
	fread(&m_fAlpha,4,1,fp);
	int n;
	fread(&n,4,1,fp);
	if(n>0)	
	{
		m_CompressTextureImage.SetSize(n);
		for(int i=0;i<n;i++)
		{
			m_CompressTextureImage[i] = new CglImageData;
			m_CompressTextureImage[i]->LoadData(fp);
		}
		m_texid = TextureMap(0);
	}
	else
	{
		int ntexid = 0;
		fread(&ntexid,4,1,fp);
		if(ntexid)
		{
			m_texid = ntexid + minTexid;
			m_uType |= 1;
		}
	}
}
bool CglMaterial::BuildMipMapTexture()
{
	int i,n = m_CompressTextureImage.GetSize();
	for(i=0;i<n;i++)
	{
		if(m_CompressTextureImage[i]->m_nColor==4)
			glTexImage2D(GL_TEXTURE_2D,m_CompressTextureImage[i]->m_nLevel,m_CompressTextureImage[i]->m_nColor,m_CompressTextureImage[i]->m_Width,m_CompressTextureImage[i]->m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_CompressTextureImage[i]->m_pData);
		else if(m_CompressTextureImage[i]->m_nColor==3)
			glTexImage2D(GL_TEXTURE_2D,m_CompressTextureImage[i]->m_nLevel,m_CompressTextureImage[i]->m_nColor,m_CompressTextureImage[i]->m_Width,m_CompressTextureImage[i]->m_Height,0,GL_RGB,GL_UNSIGNED_BYTE,m_CompressTextureImage[i]->m_pData);
		else return false;
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,m_CompressTextureImage[n-1]->m_nLevel);
	return true;
}
bool CglMaterial::BuildMipMapTexture(int colnum,int width,int height,unsigned char *pData)
{
	if(colnum<3||colnum>4)	//ֻ����3��4�ֽ���ɫ��ͼ
		return false;
	int widthout,heightout,widthPow,heightPow;
	widthPow = int(log(width+0.01f)/log(2.0f)+0.5);		
	heightPow = int(log(height+0.01f)/log(2.0f)+0.5);
	widthout = pow(2.0f,widthPow);
	heightout = pow(2.0f,heightPow);
	
	int minPow = min(widthPow,heightPow);
	int level = 0;
	unsigned char *pDataOut = new unsigned char[widthout*heightout*colnum];
	while(minPow>0)
	{
		CglImageData *image = new CglImageData;
		image->m_Width = widthout;
		image->m_Height = heightout;
		image->m_nColor = colnum;
		image->m_nLevel = level;
		image->m_pData = 0;
		if(colnum==4)
		{
			gluScaleImage(GL_RGBA,width,height, GL_UNSIGNED_BYTE,pData,widthout,heightout,GL_UNSIGNED_BYTE,pDataOut);
		//	glTexImage2D(GL_TEXTURE_2D,level++,4,widthout,heightout,0,GL_COMPRESSED_RGBA,GL_UNSIGNED_BYTE,pDataOut);
			glTexImage2D(GL_TEXTURE_2D,level++,4,widthout,heightout,0,GL_RGBA,GL_UNSIGNED_BYTE,pDataOut);
		}
		else
		{
			gluScaleImage(GL_RGB,width,height, GL_UNSIGNED_BYTE,pData,widthout,heightout,GL_UNSIGNED_BYTE,pDataOut);
		//	glTexImage2D(GL_TEXTURE_2D,level++,3,widthout,heightout,0,GL_COMPRESSED_RGB,GL_UNSIGNED_BYTE,pDataOut);
			glTexImage2D(GL_TEXTURE_2D,level++,3,widthout,heightout,0,GL_RGB,GL_UNSIGNED_BYTE,pDataOut);
		}
	/*	int compFlag;
		glGetTexLevelParameteriv(GL_TEXTURE_2D,image->m_nLevel,GL_TEXTURE_COMPRESSED,&compFlag);
		if(compFlag==GL_TRUE)	//ѹ���ɹ�
		{
			glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_INTERNAL_FORMAT,&image->m_iCompressedFormat);
			glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&image->m_nCompressedSize);
			image->m_pData = new unsigned char[image->m_nCompressedSize];
			glGetCompressedTexImage(GL_TEXTURE_2D,GL_COLOR_TABLE_FORMAT,image->m_pData);
		}
		else
		*/
		{
			image->m_nCompressedSize = 0;
			image->m_pData = new unsigned char[widthout*heightout*colnum];
			memcpy(image->m_pData,pDataOut,widthout*heightout*colnum);
		}
		if(widthPow>0)
			widthPow--;
		if(heightPow>0)
			heightPow--;
		minPow = min(widthPow,heightPow);
		widthout = pow(2.0f,widthPow);
		heightout = pow(2.0f,heightPow);
		m_CompressTextureImage.Add(image);
	}
	delete pDataOut;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,level-1);
	return true;
}
unsigned int CglMaterial::TextureMap(CglImageData * TextureImage)
{
	unsigned  texture=0;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
//	glHint(GL_TEXTURE_COMPRESSION_HINT,GL_NICEST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // �����˲�
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR); // �����˲�
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
/*	if(TextureImage->m_nColor==4)
		gluBuild2DMipmaps(GL_TEXTURE_2D,  4,
		TextureImage->m_Width, TextureImage->m_Height,
		GL_RGBA, GL_UNSIGNED_BYTE, TextureImage->m_pData);
	else if(TextureImage->m_nColor==3)
		gluBuild2DMipmaps(GL_TEXTURE_2D,  3,
		TextureImage->m_Width, TextureImage->m_Height,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->m_pData);
	else texture=0;
	*/
	if(TextureImage==0)
	{
		if(!BuildMipMapTexture())
		{
			glDeleteTextures(1,&texture);
			texture=0;
		}
	}
	else if(!BuildMipMapTexture(TextureImage->m_nColor,TextureImage->m_Width, TextureImage->m_Height,TextureImage->m_pData))
	{
		glDeleteTextures(1,&texture);
		texture=0;
	}
	return texture;
}

void CglMaterial::Render(void)
{
	if(m_uType&64)	//͸��
	{
		glEnable(GL_BLEND);	
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	if(m_uType&1)	//�Ƿ�������
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texid);

		//�����Ҫʹ��100%��������Ӧ��ʹ��ȡ���������������ȫ�ղ����������ˡ�
		//���ʹ��0%��������Ӧ����ȫʹ�ù��ն�����������
		//����������ʹ������100%Ҳ���ܹ��յ�Ӱ�죬���ԡ��������ɣ������ˣ�Ӱ�첻����඼�ܹ���Ӱ���¡�
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//��������պϳɵķ���
		float abc[4]={1,1,1,1};
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,abc);//��ʱ�����Ѿ�����պϳɹ��ˡ�
	}
	else	//û������ֱ��ʹ�ù���
	{
		glDisable(GL_TEXTURE_2D);
		if(m_uType&2)	//m_ambient
		{
			glMaterialfv(GL_FRONT,GL_AMBIENT,m_ambient);
		}
		if(m_uType&4)	//m_diffuse
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE,m_diffuse);
		}
		if(m_uType&8)	//m_specular
		{
			glMaterialfv(GL_FRONT,GL_SPECULAR,m_specular);
		}
		if(m_uType&16)	//m_emission
		{
			glMaterialfv(GL_FRONT,GL_EMISSION,m_emission);
		}
		if(m_uType&32)	//m_shininess
		{
			glMaterialfv(GL_FRONT,GL_SHININESS,&m_shininess);
		}
	}
}

void CglMaterial::SetGamma(int id, float r, float g, float b)
{
	float *col;
	if(id==0)
		col = m_ambient;
	else if(id==1)
		col = m_diffuse;
	else if(id==2)
		col = m_specular;
	else if(id==3)
		col = m_emission;
	col[0]+=r;
	col[1]+=g;
	col[2]+=b;
	for(int i=0;i<3;i++)
		if(col[i]>1)col[i]=1;
}
bool CglMaterial::CompareName(CString name)
{
	if(!name.Compare(m_name))
		return true;
	else return false;
}