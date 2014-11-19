/********************************************************************
Copyright (C) 2008 Ҧ��- All Rights Reserved
This file is part of Softcore.

Softcore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Softcore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Softcore.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

//��Ⱦ�����Ĵ��������Կ�Ҫ������ڲ����ݺ�״̬
//��vertex buffer, index buffer, vertex format, back buffer��matrix stack��
//render context�����൱���Կ�����ģ��
#ifndef _RENDER_CONTEXT
#define _RENDER_CONTEXT
#include "stdafx.h"
#include "d3d9.h"
#include "softrender_def.h"
class CSoftrender;
#include <stack>
#include <vector>
using namespace std;
struct t_contextinfo
{
	//��ʾ�ֱ��ʡ�������ʾ�ֱ���<ʵ�ʴ��ڳߴ磬���������ʱ��һ�������൱������m_win_width/m_width ����С
	int width,height;
	//ʵ��windows�ߴ�
	int win_height,win_width;
	//��ɫ��ֵ,bit per pixel
	int bpp;
	//windows info
	D3DPRESENT_PARAMETERS frminfo;
	//render device
	CSoftrender* device;
};
struct t_internal_renderop
{
	//��ͬVB��ʽ��BUFFER
	CVertexBuffer* m_vb_internal[VB_MAX_FMT];
	//��ǰ��䵽������
	int m_vb_fill_curpos[VB_MAX_FMT];
	
	//Ҫ���Ƶ�ͼԪ
	vector<t_primitive> m_primitives;
	vector<unsigned int> m_indexbuffer;
};

class CSoftrender;

class CRenderContext
{
public :
	CRenderContext(const t_contextinfo& context_param);
	virtual ~CRenderContext();
	void create();
	const t_contextinfo* getParam()const {return &m_context_param;};
	//off screen RENDERING SURFACE
	//ƽ̨��أ�����Ϊ�麯������������direct3d9 surfaceʵ�ֵ���Ⱦbuffer
	//������ƽ̨����ֱ��ʹ��DirectDraw��gdi, directly access buffer ����
	virtual void setPBuffer(void * surface){m_drawSurface = (IDirect3DSurface9*)surface;};
	virtual void* getPBuffer(){return (void*)m_drawSurface;};
	//MATRIX FUNCTIONS
	void loadIdentity(MATRIX_MODE mode);
	void setMatrix(MATRIX_MODE mode,const t_matrix& mat);
	void setMatrixMode(MATRIX_MODE mode);
	void setActiveTexture(int unit);
	t_matrix& getModeMatrix(MATRIX_MODE);
	//����context
	int reset();
	//����projection matrix
	void setPerspective(float fovx,float ratio,float n,float f);
	

	//////////////////////////////////////////////////////////////////////////
	//��context�ڲ�BUFFER�����primitive����
	//////////////////////////////////////////////////////////////////////////
	void addPrimitiveList(t_entity* ent,t_mesh* mesh ,t_camera* cam);

	//���¸ı��ڲ�VB��С
	void resizeRenderVB(int num,VB_FMT fmt);
	//����ڲ�vb�������vb�Ǿ���ת���Ķ������꣬�����screen space��λ��
	void cleanInternalVB();
	//�õ��ڲ�vb
	char* getVBuffer(VB_FMT);
	//���ӵ�ǰ���λ��
	int appendFillPos(int num,VB_FMT);

	//ת��entity ���㵽screen space��x,y����Ļ�������꣬ͬʱ����device space��z��w
	int tranformAndFillPos(t_entity* ent,int numVert,t_drawVert* vb,VB_FMT vb_format);
	int tranformAndFillPosNorm(t_entity* ent,int numVert,t_drawVert* vb,VB_FMT vb_format);
	//������triangle�����դ��
	int pushTriangleListToRasterizer();
	//��סsurface��ʼд�� ��ƽ̨���
	virtual void* lockSurface(int& linewidth);
	virtual void unlockSurface();
	//�õ�surface �ߴ�
	int getSurfaceWidth()const{return m_context_param.width;};
	int getSurfaceHeight()const{return m_context_param.height;};
	//�õ�viewport��������ָ��,just for convenience
	float* getViewport(){return m_viewport;};
	void setViewport(const int * );

	//����һ��ȫ����Ⱦ��ɫ
	int setGlobalColor(sfByte8*);
	sfByte8* getGlobalColor(){return m_global_color;};
	ZBUF_TYPE* getZBuffer()const{return m_zbuffer;};
	void setBackfaceCulling(int cull){m_backfaceCulling=cull;};
	int getBackfaceCulling()const{return m_backfaceCulling;};
protected:
	
	//�û�����ת������ȾBUFFER��Ϊ��ֻ����һ���ڴ�copy����back face culling, near plane triangle clipping��
	//��vertex transform ������һ����������������ֱ�Ӵ��û���mesh vb����һ����ת����context�ڲ���ȾVERTEX BUFFER��
	int backCullingAndClipAndTransform(t_entity* ent, t_mesh* mesh ,t_primitive* primitive,t_camera* cam);

	//context param
	t_contextinfo m_context_param;
	
	//offscreen buffer
	IDirect3DSurface9* m_drawSurface;

	//model matrix,view matrix, projection matrix, texture matrixs, all kinds of matrix stack, 
	vector<t_matrix> viewmatrix;
	vector<t_matrix> projmatrix;
	vector<t_matrix> texmatrix[MAX_STAGE];

	int iView,iProj,iTex[MAX_STAGE];
	MATRIX_MODE m_mat_mode;
	int m_unit;
	//view port, x,y, w,h
	float m_viewport[4];
	
	//global color
	sfByte8 m_global_color[4];
	//0, no culling, 1, CW culling, 2, CCW culling
	int m_backfaceCulling;

	//����ʱ���ڲ�vb���ݣ�����ת����Ķ���ζ�������������buffer
	//internal renderopҲ��¼�˵�ǰ����buffer��λ�ã��Լ�Ҫ���Ƶ�primitive��Ϣ
	t_internal_renderop m_renderop;
	ZBUF_TYPE *m_zbuffer;

private:
	CRenderContext();
};


#endif