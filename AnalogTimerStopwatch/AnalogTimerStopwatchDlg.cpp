
// AnalogTimerStopwatchDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "AnalogTimerStopwatchDlg.h"
#include "afxdialogex.h"
#include "SettingTimerDlg.h"
#include "SettingSystemTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Tab1.h"
#include "Tab2.h"

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CAnalogTimerStopwatchDlg ��ȭ ����

CAnalogTimerStopwatchDlg::CAnalogTimerStopwatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANALOGTIMERSTOPWATCH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnalogTimerStopwatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}

BEGIN_MESSAGE_MAP(CAnalogTimerStopwatchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CAnalogTimerStopwatchDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_32771, &CAnalogTimerStopwatchDlg::On32771)
	ON_COMMAND(ID_32772, &CAnalogTimerStopwatchDlg::On32772)
END_MESSAGE_MAP()

HHOOK g_hook = NULL;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPKBDLLHOOKSTRUCT pkbhs = (LPKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0 && pkbhs->vkCode == 'X')
		{
			DWORD dwFlags = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
			if (dwFlags == 0)
			{
				AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
				return TRUE;
			}
		}
	}
	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

// CAnalogTimerStopwatchDlg �޽��� ó����

BOOL CAnalogTimerStopwatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetHook();

	mTab.InsertItem(0, _T("Timer"));
	mTab.InsertItem(1, _T("Stopwatch"));

	mTab.SetCurSel(0);

	CRect rect;
	mTab.GetWindowRect(&rect);

	mDlg1 = new CTab1;
	mDlg1->Create(IDD_DIALOG1, &mTab);
	mDlg1->MoveWindow(0, 0, rect.Width(), rect.Height() - 23);
	mDlg1->ShowWindow(SW_SHOW);

	mDlg2 = new CTab2;
	mDlg2->Create(IDD_DIALOG2, &mTab);
	mDlg2->MoveWindow(0, 0, rect.Width(), rect.Height() - 23);
	mDlg2->ShowWindow(SW_HIDE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CAnalogTimerStopwatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAnalogTimerStopwatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CAnalogTimerStopwatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAnalogTimerStopwatchDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int sel = mTab.GetCurSel();

	switch (sel) {
	case 0:
		mDlg1->ShowWindow(SW_SHOW);
		mDlg2->ShowWindow(SW_HIDE);
		break;
	case 1:
		mDlg1->ShowWindow(SW_HIDE);
		mDlg2->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}


void CAnalogTimerStopwatchDlg::On32771()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSettingTimerDlg dlg;
	dlg.DoModal();
}

BOOL CAnalogTimerStopwatchDlg::SetHook()
{
	if (g_hook != NULL)
		UnHook();
	g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, (HINSTANCE)theApp.m_hInstance, NULL);

	return TRUE;
}

BOOL CAnalogTimerStopwatchDlg::UnHook()
{
	if (g_hook != NULL)
	{
		UnhookWindowsHookEx(g_hook);
	}
	return TRUE;
}


BOOL CAnalogTimerStopwatchDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	UnHook();

	return CDialogEx::DestroyWindow();
}


void CAnalogTimerStopwatchDlg::On32772()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSettingSystemTimeDlg dlg;
	dlg.DoModal();
}
