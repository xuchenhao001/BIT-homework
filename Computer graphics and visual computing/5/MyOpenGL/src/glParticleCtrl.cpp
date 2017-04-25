// glParticleCtrl.cpp: implementation of the CglParticleCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "glParticleCtrl.h"
#include "OpenGL.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CglParticleCtrl::CglParticleCtrl()
{

}

CglParticleCtrl::~CglParticleCtrl()
{

}

void CglParticleCtrl::Init(int num)
{
	m_numParticle = num;
	int i;
	m_lasttime = GetTickCount();
	srand(m_lasttime);
	for(i=0;i<m_numParticle;i++)
	{
		ResetParticle(i,true);
	}
	//加入粒子纹理
	CglMaterial material;
	m_textureid = material.LoadGLTextures("火焰.bmp");
}

void CglParticleCtrl::UpdatePos()
{
	double mat[16],imat[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,mat);
	CglMath::InverseMatrix(mat,imat);
	CglVector3 eyepos = imat*CglVector3(0,0,0);
	int i;
	int curtime = GetTickCount();
	float usetime = curtime - m_lasttime;
	m_lasttime = curtime;
	for(i=0;i<m_numParticle;i++)
	{
		if(m_particle[i].m_bActive)	//活的粒子
		{
			m_particle[i].m_lifetime-=1;
			if(m_particle[i].m_lifetime<=0)
				m_particle[i].m_bActive=false;
			m_particle[i].m_dSpeed+=usetime*m_particle[i].m_acc;
			if(m_particle[i].m_dSpeed<0)
				m_particle[i].m_dSpeed=0;
			m_particle[i].m_pos += usetime*m_particle[i].m_particleMoveDir*m_particle[i].m_dSpeed/1000;

			m_particle[i].m_color[3]-=m_particle[i].m_colorfade;

		}
		else	//重新激活粒子
		{
			ResetParticle(i);
		}
		m_particle[i].BillBoard(eyepos);
	}
}

void CglParticleCtrl::Draw()
{
	glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	//打开混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_textureid);
	for(int i=0;i<m_numParticle;i++)
	{
		if(m_particle[i].m_bActive)	//活的粒子
			m_particle[i].Draw();
	}
	glPopAttrib();
}

void CglParticleCtrl::ResetParticle(int id,bool bFirst)
{
	m_particle[id].m_bActive=true;
	m_particle[id].m_pos.Set(0,0,0);	//初始位置

	m_particle[id].m_lifetime=(double(rand())/RAND_MAX)*20;
	double seta = double(rand())*2*PAI/RAND_MAX;
	double beta = double(rand())*PAI/30/RAND_MAX;
	m_particle[id].m_particleMoveDir[0]=sin(beta)*cos(seta);
	m_particle[id].m_particleMoveDir[1]=cos(beta);
	m_particle[id].m_particleMoveDir[2]=sin(beta)*sin(seta);

	m_particle[id].m_dSpeed=double(rand())/RAND_MAX*90+20;		//粒子运动速度
	m_particle[id].m_acc=-double(rand())/RAND_MAX*0.2-0.4;
	m_particle[id].m_size=0.5;
	m_particle[id].m_color[3]=0.1;
	m_particle[id].m_colorfade = m_particle[id].m_color[3]/m_particle[id].m_lifetime;
}
