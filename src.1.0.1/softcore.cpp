// softcore.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "softcore.h"
#include "MainFrm.h"
#include ".\softcore.h"
#include "mmgr/mmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsoftcoreApp

BEGIN_MESSAGE_MAP(CsoftcoreApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CsoftcoreApp ����

CsoftcoreApp::CsoftcoreApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsoftcoreApp ����

CsoftcoreApp theApp;

// CsoftcoreApp ��ʼ��

BOOL CsoftcoreApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ش�������Դ�Ŀ��
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}


// CsoftcoreApp ��Ϣ�������



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CsoftcoreApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CsoftcoreApp ��Ϣ�������


BOOL CsoftcoreApp::OnIdle(LONG lCount)
{

	return CWinApp::OnIdle(lCount);
}

int CsoftcoreApp::Run()
{
	OutputDebugStringA("App start running\n");

	m_canIdle=true;
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	MSG msgCur;


	for (;;)
	{
		{
			// check to see if we can do idle work
			if(bIdle&&!::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE))
			{
				if (!OnIdle(lIdleCount++) && m_canIdle)
					bIdle = FALSE; // assume "no idle" state

				
			}
			else
			{
				// do the message handling 
				do
				{
					if (!PumpMessage())
						return ExitInstance();

					if (IsIdleMessage(&msgCur))
					{
						bIdle = TRUE;
						lIdleCount = 0;
					}	
				}while(::PeekMessage(&msgCur, NULL, NULL, NULL, PM_NOREMOVE));

				if (m_canIdle)
					((CMainFrame*)AfxGetApp()->GetMainWnd())->RunFrame();
			}

			
			if (!m_canIdle)
				((CMainFrame*)AfxGetApp()->GetMainWnd())->RunFrame();

		}
	}
}
