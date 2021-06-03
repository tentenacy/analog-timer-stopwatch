// Tab1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "Tab1.h"
#include "afxdialogex.h"
#include "AnalogTimerStopwatchDlg.h"
#include "SettingTimerDlg.h"

#define IDE_SYSTEM_COUNTER 1000


// CTab1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab1, CDialogEx)

CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, mHour(0)
	, mMinute(0)
	, mSecond(0)
	, mSystemCounter(NULL)
	, isTimerStarted(FALSE)
	, mThreadTimer(NULL)
	, mProgressSweepAngle(0)
	, mTimer(NULL)
{

}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUR, mHour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mMinute);
	DDX_Text(pDX, IDC_EDIT_SECOND, mSecond);
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTab1::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_START, &CTab1::OnBnClickedButtonStart)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(THREAD_TIMER, TimerThreadUpdate)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CALLBACK SystemCounterCallback(HWND hWnd, UINT nID, UINT nEl, DWORD time) {
	CWnd* pWnd = AfxGetMainWnd();
	CAnalogTimerStopwatchDlg* pParent = (CAnalogTimerStopwatchDlg*)pWnd;

	pParent->mDlg1->mSystemCounter->TicTok();
	pParent->mDlg1->GetDlgItem(IDC_STATIC_COUNTER)->SetWindowTextW(pParent->mDlg1->mSystemCounter->GetTimeFormatted());
}

UINT TimerThread(LPVOID param)
{
	ThreadTimerParam* pParam = (ThreadTimerParam*)param;
	CTab1* pMain = (CTab1*)pParam->hwnd;
	CustomTimer* timer = pParam->timer;

	ThreadTimerUpdateParam threadTimerUpdateParam;

	::PostMessage(pMain->m_hWnd, THREAD_TIMER, THREAD_UPDATE_TIMER, (LPARAM)&threadTimerUpdateParam);

	clock_t start = clock();
	clock_t end = clock();
	clock_t prevEnd;
	clock_t compConst = 0;
	for (int i = 1; pMain->mThreadTimerWork == THREAD_RUNNING; ) {
		prevEnd = end;
		end = clock() + compConst;
		if (double(end - prevEnd) > 50) {
			compConst -= end - prevEnd;
			end -= end - prevEnd;
		}
		
		threadTimerUpdateParam.time = double(end - start);
		::PostMessage(pMain->m_hWnd, THREAD_TIMER, THREAD_UPDATE_PROGRESS, (LPARAM)&threadTimerUpdateParam);
		Sleep(10);
		if (double(end - start) >= 1000 * i) {
			i += 1;
			timer->TicTok();
			::PostMessage(pMain->m_hWnd, THREAD_TIMER, THREAD_UPDATE_TIMER, (LPARAM)&threadTimerUpdateParam);
			if (timer->IsTimeOut()) {
				Sleep(100);
				break;
			}
		}
	}
	::PostMessage(pMain->m_hWnd, THREAD_TIMER, THREAD_UDPATE_TIMEOUT, (LPARAM)&threadTimerUpdateParam);
	return 0;
}

LRESULT CTab1::TimerThreadUpdate(WPARAM wParam, LPARAM lParam)
{

	ThreadTimerUpdateParam* pParam = (ThreadTimerUpdateParam*)lParam;
	switch ((UINT)wParam) {
	case THREAD_UPDATE_TIMER:
		GetDlgItem(IDC_STATIC_TIMER)->SetWindowTextW(mTimer->GetTimeFormatted());
		break;
	case THREAD_UPDATE_PROGRESS:
		if (mProgressSweepAngle >= 0) {
			mProgressSweepAngle = 360 - 360 * (pParam->time / 1000 / (mHour * 60 * 60 + mMinute * 60 + mSecond));
		}
		else {
			mProgressSweepAngle = 0;
		}
		OnDrawProgressImage();
		break;
	case THREAD_UDPATE_TIMEOUT:
		isTimerStarted = FALSE;
		HideTimer();
		mThreadTimerWork = THREAD_STOP;
		Invalidate(TRUE);
		mProgressSweepAngle = 0;
		break;
	}
	
	return 0;
}

// CTab1 메시지 처리기입니다.


void CTab1::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// SetBackgroundColor(Color::White);

	CFont *font = this->GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);
	lf.lfHeight = 20;
	this->mFont.CreateFontIndirectW(&lf);
	GetDlgItem(IDC_STATIC_COUNTER)->SetFont(&this->mFont);
	mFont.Detach();
	lf.lfHeight = 80;
	this->mFont.CreateFontIndirectW(&lf);
	GetDlgItem(IDC_STATIC_TIMER)->SetFont(&this->mFont);
	mFont.Detach();

	GetDlgItem(IDC_PC_PROGRESS)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_COUNTER)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_TIMER)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_COUNTER)->BringWindowToTop();
	GetDlgItem(IDC_STATIC_TIMER)->BringWindowToTop();

	mSystemCounter = new CustomSystemCounter;
	GetDlgItem(IDC_STATIC_COUNTER)->SetWindowTextW(mSystemCounter->GetTimeFormatted());
	SetTimer(IDE_SYSTEM_COUNTER, 1000, SystemCounterCallback);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTab1::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	isTimerStarted = FALSE;
	StopTimer();
}


void CTab1::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!mHour && !mMinute && !mSecond) return;
	if (!isTimerStarted) {
		isTimerStarted = TRUE;
		StartTimer();
		return;
	}
	if (mThreadTimerWork == THREAD_RUNNING) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("재개"));
		mThreadTimer->SuspendThread();
		mThreadTimerWork = THREAD_PAUSE;
	}
	else if (mThreadTimerWork == THREAD_PAUSE) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("일시정지"));
		mThreadTimer->ResumeThread();
		mThreadTimerWork = THREAD_RUNNING;
	}
}


void CTab1::ShowTimer()
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_PC_PROGRESS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_TIMER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_COUNTER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_HOUR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MINUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HOUR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MINUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SECOND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("일시정지"));
}


void CTab1::HideTimer()
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_PC_PROGRESS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TIMER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_COUNTER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_HOUR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MINUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_HOUR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MINUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SECOND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("시작"));
}

void CTab1::StartTimer()
{
	ShowTimer();
	mThreadTimerParam.hwnd = this;
	mTimer = new CustomTimer(mHour, mMinute, mSecond);
	mThreadTimerParam.timer = mTimer;

	mThreadTimer = AfxBeginThread(TimerThread, &mThreadTimerParam);
	mThreadTimerWork = THREAD_RUNNING;
}


void CTab1::StopTimer()
{
	HideTimer();

	mThreadTimer->SuspendThread();
	DWORD dwResult;
	::GetExitCodeThread(mThreadTimer->m_hThread, &dwResult);

	delete mThreadTimer;
	delete mTimer;
	mThreadTimer = NULL;
	mTimer = NULL;

	mThreadTimerWork = THREAD_STOP;
	Invalidate(TRUE);
	mProgressSweepAngle = 0;
}


void CTab1::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(IDE_SYSTEM_COUNTER);
	delete mSystemCounter;
}

void CTab1::OnDraw(CDC* pDC) {
	if (mThreadTimerWork == THREAD_STOP) {
		SetBackgroundColor(Color::White);
	}
}


void CTab1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	OnDraw(&dc);
	CDialogEx::OnPaint();
}


HBRUSH CTab1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CBrush brh;
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	brh.CreateSolidBrush(RGB(255, 255, 255));

	if ((nCtlColor == CTLCOLOR_STATIC) || (nCtlColor == CTLCOLOR_EDIT))
		pDC->SetBkColor(RGB(255, 255, 255));	//배경색 변경
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(255, 255, 255));	//배경색 변경
		return brh;
	}
	else
	{
		if (nCtlColor == CTLCOLOR_BTN)
			pDC->SetBkMode(TRANSPARENT);		//배경 투명하게
	}
	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}


void CTab1::OnDrawProgressImage()
{
	// Picture Control DC를 생성.
	CClientDC dc(GetDlgItem(IDC_PC_PROGRESS));	

	// Picture Control 크기를 얻는다.
	CRect rect;
	GetDlgItem(IDC_PC_PROGRESS)->GetClientRect(&rect);

	// Picture Control에 호환되는 Graphics 생성.
	Graphics mainG(dc.GetSafeHdc());

	// Picture Control 크기로 비트맵(임시 버퍼) 생성.
	Bitmap memBmp(rect.Width(), rect.Height());

	// 임시 버퍼에 호환되는 Graphics 생성.
	Graphics memG(&memBmp);

	memG.Clear(Color::White);

	// 그리는 작업.
	Pen DarkGrayPen(Color(255, 38, 37, 44), 3);
	Pen OrangePen(Color(255, 251, 160, 11), 3);
	OrangePen.SetWidth(8.0f);
	DarkGrayPen.SetWidth(7.5f);
	OrangePen.SetStartCap(LineCapRound);
	OrangePen.SetEndCap(LineCapRound);
	memG.SetSmoothingMode(SmoothingModeAntiAlias);
	memG.DrawArc(&DarkGrayPen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, 0, 360);
	memG.DrawArc(&OrangePen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, -90, mProgressSweepAngle);
	// 임시 버퍼를 Picture Control에 그리기.
	mainG.DrawImage(&memBmp, 0, 0);
}