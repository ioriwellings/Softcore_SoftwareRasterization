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

//������Ⱦ������
#ifndef _SOFTRENDER
#define _SOFTRENDER
#include <d3d9.h>
#include "softrender_def.h"
#include "vertexBuffer.h"
#include <vector>

class CVertexBuffer;
using namespace std;
class CRenderContext;
class CSoftrender
{
public:
	static CSoftrender* instance();
	static void destroy();
	
public:
	int getState();
	int initSystem(HWND hWnd,int w,int h);
	int shutdownSystem();

	//realtime utility
	int clearScreen(int flags,int r,int g,int b,int a,float z,DWORD stencil);
	int renderFrame();
	int swap();
	int frameBegin();
	int frameEnd();

	//util
	int createContext(int w,int h);
	int releaseContext();
	CRenderContext* getContext(int n);
	void setContext(int n);
	int createPBuffer(CRenderContext*);
	int destroyPBuffer(CRenderContext*);
	int activeContext(int n);

	//transform pipeline,notice: use right-hand coordination
	//�������λ�ó������׶ƽ����
	void setCamera(const vec3& eyepos,const vec3& lookat,const vec3& up);
	t_camera& getCamera() ;
	//��ͷ��Ƕȣ���Ļ���������Զƽ����룬��ƽ�����
	void setFrustum(float fovx,float ratio,float n,float f);
	//ÿ֡��������ľ���
	void updateCamera();
	//viewport update
	void setViewport(const int* vp);
	void getViewport(int* vp);

	//geometry pipeline
	int pushTriangle(const t_triangle& tri);
	int pushEntity(t_entity* entity);
	//����transform ����Ľ�����,һ��vertex formatһ��vb�ȿ�
	int setDynamicVB(CVertexBuffer* vb);
	//transform��Ϻ�ѹ����Ⱦ�ܵ����ȴ�����դ��
	int pushDynmaicVB(CVertexBuffer* vb);
	//culling in world space use entity's aabb or bounding sphere 
	int frustumCulling();

	//add a light
	int setLight(int idx,vec4 pos,vec4 col);
	//add a light 
	void addLight(clight_directional*);
	//ת���ƹ�
	int processLighting();
	//����ת��������pos, normal, T,B, ��������Ҫ��Ⱦ��primitiveѹ��context
	int processEntity();
	//begin rasterizer
	int rasterizer();
	

	//render state setup
	int setTexture();
	int setRenderState();
	int setTextureStage();
	//����rgba����ɫ��ֵ,r,g,b,a��˳��
	int setGlobalColor(vec4);

	//primitive rendering
	int drawPrimitive();
	int drawTriangle();

	//resource manangement
	int createSwapWindowBuffer(DWORD w,DWORD h);
	int releaseSwapChain();
	CVertexBuffer* createVBuffer(int, VB_FMT);

	//draw utility
	//���÷���ת����ֵ
	void setRasterizierAffine(int );
	//get affine status
	int getRasterizierAffine();
	//set rasterizier flags
	void setRasterizierFlags(int flags);
	//get rasterizier flags
	int  getRasterizierFlags();

	void setCulling(int cull);
	int getCulling()const;
	int drawText(const char* str, int x,int y);
	int drawPoint(dpoint* p);
	int printFPS();
	unsigned int getFrameInterval(){return m_deltaTime;};
	unsigned int getFrameCount()const{return m_frm;};
private:
	CSoftrender();
	~CSoftrender();
protected:
	//util
	void checkEverything();

protected:
	IDirect3D9* m_d3d9;
	IDirect3DDevice9* m_device;
	IDirect3DSwapChain9* m_swapChain;
	IDirect3DSurface9* m_frmSurface,*m_depthSurf,*m_drawSurface;
	D3DPRESENT_PARAMETERS m_frmParam; 

	//��ǰ��Ⱦcontext
	CRenderContext* m_context;
	vector<CRenderContext*> m_context_mng;
	CVertexBuffer* m_dyn_vb;

	//��Ⱦ������ÿ֡���
	vector<t_entity*> m_entity;
	//��ʱ����
	t_matrix* m_mat_temp;
	//������
	unsigned int m_frm;
	//��һ����Ⱦѭ�������󣬱���ִ��frameEnd()
	unsigned int m_frmBegin,m_frmEnd;
	//current valid light
	vector<clight_directional*> m_lights;
	t_camera m_camera;
	unsigned int  m_deltaTime;

};
#endif