// softcore.h : softcore Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CsoftcoreApp:
// �йش����ʵ�֣������ softcore.cpp
//

class CsoftcoreApp : public CWinApp
{
public:
	CsoftcoreApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

	virtual int Run();


public:
	void setIdleState(bool idle){m_canIdle = idle;};
	bool getIdleState()const{return m_canIdle;};
protected:
	//always refresh the screen or can idle
	bool m_canIdle;
};

extern CsoftcoreApp theApp;
