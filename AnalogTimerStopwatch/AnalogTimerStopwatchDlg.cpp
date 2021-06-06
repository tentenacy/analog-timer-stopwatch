
// AnalogTimerStopwatchDlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// CAnalogTimerStopwatchDlg 대화 상자

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

// CAnalogTimerStopwatchDlg 메시지 처리기

BOOL CAnalogTimerStopwatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAnalogTimerStopwatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAnalogTimerStopwatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAnalogTimerStopwatchDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UnHook();

	return CDialogEx::DestroyWindow();
}


void CAnalogTimerStopwatchDlg::On32772()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSettingSystemTimeDlg dlg;
	dlg.DoModal();
}
