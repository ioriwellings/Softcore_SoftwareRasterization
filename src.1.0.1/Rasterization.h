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
//��դ��ģ��

#ifndef _RASTERIZATION
#define _RASTERIZATION

#include "softrender_def.h"
class CRenderContext;

//��դ����Ļ��������
class t_rasterizier_point 
{
public:
	//����ȱʡ�˹���Ϳ������죬ֱ�Ӱ�bit copy
	//t_rasterizier_point ();
	//t_rasterizier_point (t_rasterizier_point& );

	//��Ļ���ش�ֱ��ֵ
	sfINLINE void interpolateVerticalCol();//gouraud shading without texture
	sfINLINE void interpolateVerticalTex();//gouraud shading with texture
	sfINLINE void interpolateVerticalColTex();//gouraud shading with texture
	sfINLINE void interpolateVerticalTBNTex();//phong shading with texture

	//vertex point
	vec4 pl,pr,dpl,dpr;
	//tagent space
	vec3 T,B,N;
	//vertex color, �������Ѿ�/w ����ֵ
	vec4 cl,cr,dcl,dcr;
	vec4 tl,tr,dtl,dtr;
	//perspective correction texture mapping & project texture,Ҳ�Ѿ��� /w ������ֵ

	//����ʡ�Զ�����ͼ
	//	vec4 tl[MAX_STAGE],tr[MAX_STAGE],dtl[MAX_STAGE],dtr[MAX_STAGE];
};

/*////////////////////////////////////////////////////////////////////////
 ����oo����Ӧ�ó�����Ϊ�仯���ѻ���
tex triangle, 
color triangle, 
tex+col triangle, 
phong triangle, 
bump phong triangle
����Ϊ����Ϊ��ͬ����
����Ϊ��դ����ȫ���ٶ����ȣ�
ʵ��ɨ���ߺ�����Ӧʹ��SIMD������ʵ��
�˴���դ��Ϊ��ʵ�ּ򻯣�ʹ�ò�ͬ��������ʵ�ֲ�ͬshadingʱ��ɨ�����㷨
//////////////////////////////////////////////////////////////////////////*/
class CRasterizer
{
	CRasterizer();
	virtual ~CRasterizer();
	CRenderContext* m_context;
	
public:
	//�ڲ�buffer
	//TODO,ÿ��vb format����ӵ��һ��buffer
	struct t_rast_triangle
	{
		//�Ѿ����и����3����
		vec4 point[3];
		vec4 col[3];
		vec2 tex[3];
		vec3 norm[3];
		vec3 T[3],B[3];
		const t_primitive* pri;		
	};



	//TODO: ʹ�����¸�ʽʹ����128bit���룬ʹ�ò�ֵ���������SIMD���
	/*
	struct t_rast_triangle
	{
	//�Ѿ����и����3����
	vec3 point[3];
	vec3 norm[3];
	vec2 uv[3];
	vec4 col[3];
	}
	t_rast_triangle* m_tirs = (t_rast_triangle*)aligned_alloc(sizeof( t_rast_triangle)* num, 16);
	t_primitive* m_primitives = new t_primitive[num];
	*/
	static CRasterizer& instance();

	//���õ�ǰcontext
	void setContext(CRenderContext* );

	//���ⲿ�仯context viewportʱ���������rasterizier��viewport
	void updateViewport();

	//���������Ⱦ��triangle list������ͨ�������и����ƽ�������κ���ƽ��������
	int pushTriangle(const t_primitive&);

	//!��դ��������
	int drawTriangle_colTex(vector<t_rast_triangle>& ,bool );


	/*
	screen space����Ҫ��ֱ�и���������� _xclip ��������
	Ϊ�˼򻯣�ֻ�ǰ�Ҫ��ֵ�������ԵĶ��ٲ������ֻ��ƺ���������������ÿһ��
	�ж���Щ����������Ҫ��ֵ�����ٶ�̬��֧
	���ö�̬����ָ�����
	*/

	//!���Ƶ�ɫ�����Σ�
	template<class T>  void rastTri_constCol(int iy0,int iy1,sfByte8* buf,t_rasterizier_point attribute,T col);
	template<class T>  void rastTri_constCol_xclip(int iy0,int iy1,int minX,int maxX,sfByte8* buf,t_rasterizier_point& attribute,T col);
	//!diffuse lighting+texture triangle
	template<class T>  void rastTri_constColTex(int iy0,int iy1,sfByte8* buf,ZBUF_TYPE* zbuf,
		t_rasterizier_point attribute,T col,t_material* material);
	template<class T>  void rastTri_constColTex_xclip(int iy0,int iy1,int minX,int maxX,sfByte8* buf,ZBUF_TYPE* zbuf,
		t_rasterizier_point& attribute,T col,t_material* material);


	//!���ƹ̶���ɫɨ����
	template<class T> sfINLINE  int scanLine_constCol_Affine(T* buf,int xleft,int xright,T col,t_rasterizier_point& p);
	//!affine linear color filling
	template<class T> sfINLINE  int  scanLine_constCol(T* buf,int xleft,int xright,T col,t_rasterizier_point& p);


	//!���ƹ���+��ͼɨ����
	template<class T>  sfINLINE int scanLine_ColTex(T* buf,ZBUF_TYPE* zbuf,int xleft,int xright,T col,t_rasterizier_point& p,t_material* material);
	//!affine texture mapping
	template<class T>  sfINLINE int scanLine_ColTex_Affine(T* buf,int xleft,int xright,T col,t_rasterizier_point& p,t_material* material);

	/*
	pixel shading operations
	����ֻ������diffuse texture* diffuse color
	��constant color��pixel operation
	*/
	template<class T> sfINLINE void pixel_shader_constCol(const vec4& col,T* c);
	//!֧��16��32λɫ��ȫ�ػ��汾
	template<> sfINLINE void pixel_shader_constCol (const vec4& col,sfCol16* c);
	template<> sfINLINE void pixel_shader_constCol (const vec4& col,sfCol32* c);
	
	template<class T> sfINLINE T pixel_shader_modulateTexCol(const T col, const vec4& c);
	//!֧��16��32λɫ��ȫ�ػ��汾
	template <> sfINLINE sfCol16 pixel_shader_modulateTexCol(const sfCol16 col, const vec4& c);
	template <> sfINLINE sfCol32 pixel_shader_modulateTexCol(const sfCol32 col, const vec4& c);


	//!2d texture sampler
	template<class T> sfINLINE void tex2DSampler(const vec4& tc,int w,int h,int pitch,int bpp,sfByte8* texbuf,T& col,t_texture* tex);
	//!֧��16��32λɫ��ȫ�ػ��汾
	template<> sfINLINE void tex2DSampler(const vec4& tc,int w,int h,int pitch,int bpp,
		sfByte8* texbuf,sfCol16& col,t_texture* tex);
	template<> sfINLINE void tex2DSampler(const vec4& tc,int w,int h,int pitch,int bpp,
		sfByte8* texbuf,sfCol32& col,t_texture* tex);

	//!�������������Σ�ͬʱ���������ʱ��Ⱦ����
	int flush();
	//������й�դ���е���ʱ��Ⱦ����
	void cleanAll();
	//
	int getFlags()const{return m_run_flags;};
	void setFlags(int flags){m_run_flags = flags;};
protected:

	//��ƽ������ƽ�������Σ�һ�ָ�ʽһ��array,ÿ��������Ͷ�̬���
	vector<t_rast_triangle> m_rast_triangles[2][VB_MAX_FMT];
	void* m_surfaceBuf;
	int m_run_flags;
	float m_epslon;
	//l,r,t,b
	float* m_viewport;
	int m_iviewport[4];
	int m_linebytes;
	//ָ��context z buffer��ָ��
	ZBUF_TYPE* m_zbuf;

};
#endif