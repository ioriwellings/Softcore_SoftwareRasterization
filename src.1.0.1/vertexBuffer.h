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

//����context�ڲ���Ⱦ���㻺�����
#ifndef _VERTEXBUFFER
#define _VERTEXBUFFER
#include "softrender_def.h"
//���ڲ�ͬ��դ���ܵ�

class CVertexBuffer
{
public:
	CVertexBuffer(int ,VB_FMT);
	~CVertexBuffer();

	//��չvb����������ԭʼ��С
	int resize(int numVert);

	//should 16bytes aligned,
	//just use microsoft's _aligned_malloc (size,16) and _aligned_free  is ok
	char* m_buffer;
	VB_FMT m_format;
	int m_strip;
	int m_vert_num;
	int m_size;
private:
	CVertexBuffer();
};
class CIndexBuffer
{

};
#endif