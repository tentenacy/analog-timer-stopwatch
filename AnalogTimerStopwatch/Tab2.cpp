// Tab2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "Tab2.h"
#include "afxdialogex.h"
#include "CustomStopwatch.h"


// CTab2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab2, CDialogEx)

CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, isStopwatchStarted(FALSE)
	, mThreadStopwatch(NULL)
	, mStopwatch(NULL)
	, mWatchSecond(NULL)
	, mWatchMinute(NULL)
{

}

CTab2::~CTab2()
{
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LAB, mLabList);
}


BEGIN_MESSAGE_MAP(CTab2, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTab2::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_LAB, &CTab2::OnBnClickedButtonLab)
	ON_WM_PAINT()
	ON_MESSAGE(THREAD_STOPWATCH, StopwatchThreadUpdate)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


UINT StopwatchThread(LPVOID param)
{
	ThreadStopwatchParam* pParam = (ThreadStopwatchParam*)param;
	CTab2* pMain = (CTab2*)pParam->hwnd;
	CustomStopwatch* stopwatch = pParam->stopwatch;


	::PostMessage(pMain->m_hWnd, THREAD_STOPWATCH, THREAD_UPDATE_STOPWATCH, NULL);

	clock_t start = clock();
	clock_t end = clock();
	clock_t prevEnd;
	clock_t compConst = 0;
	for (; pMain->mThreadStopwatchWork == THREAD_RUNNING; ) {
		prevEnd = end;
		end = clock() + compConst;
		if (double(end - prevEnd) > 50) {
			compConst -= end - prevEnd;
			end -= end - prevEnd;
		}

		Sleep(2);
		stopwatch->SetTime(double(end - start));
		::PostMessage(pMain->m_hWnd, THREAD_STOPWATCH, THREAD_UPDATE_STOPWATCH, NULL);
	}
	return 0;
}


LRESULT CTab2::StopwatchThreadUpdate(WPARAM wParam, LPARAM lParam)
{

	switch ((UINT)wParam) {
	case THREAD_UPDATE_STOPWATCH:
		GetDlgItem(IDC_STATIC_STOPWATCH)->SetWindowTextW(mStopwatch->GetTimeFormatted());
		mStopwatch->SetLabTime(mStopwatch->GetTime());
		mLabList.SetItemText(0, 1, mStopwatch->GetLabTimeFormatted());
		break;
	case THREAD_UPDATE_LAB:
		
		break;
	}

	return 0;
}

// CTab2 메시지 처리기입니다.


void CTab2::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_STATIC_STOPWATCH)->SetWindowTextW(_T("00:00.00"));

	CFont *font = this->GetFont();
	LOGFONT lf;
	font->GetLogFont(&lf);
	lf.lfHeight = 20;
	this->mFont.CreateFontIndirectW(&lf);
	GetDlgItem(IDC_STATIC_STOPWATCH)->SetFont(&this->mFont);
	mFont.Detach();

	CRect rect;
	mLabList.GetClientRect(&rect);
	int width = rect.Width();
	width -= GetSystemMetrics(SM_CXVSCROLL);
	mLabList.InsertColumn(0, _T("랩"), LVCFMT_LEFT, 260);
	mLabList.InsertColumn(1, _T("기록"), LVCFMT_LEFT, width - 260);

	AllocateWatch();

	mWatchSecond->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	mWatchMinute->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_STOPWATCH)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_BUTTON_START)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_BUTTON_LAB)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	mWatchMinute->BringWindowToTop();
	GetDlgItem(IDC_STATIC_STOPWATCH)->BringWindowToTop();
	GetDlgItem(IDC_BUTTON_START)->BringWindowToTop();
	GetDlgItem(IDC_BUTTON_LAB)->BringWindowToTop();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH CTab2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CTab2::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!isStopwatchStarted) {
		isStopwatchStarted = TRUE;
		StartStopwatch();
		return;
	}

	if (mThreadStopwatchWork == THREAD_RUNNING) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("시작"));
		GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("재설정"));
		mThreadStopwatch->SuspendThread();
		mThreadStopwatchWork = THREAD_PAUSE;
		mWatchSecond->PauseWatch();
		mWatchMinute->PauseWatch();
	}
	else if (mThreadStopwatchWork == THREAD_PAUSE) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("중단"));
		GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("랩"));
		mThreadStopwatch->ResumeThread();
		mThreadStopwatchWork = THREAD_RUNNING;
		mWatchSecond->ResumeWatch();
		mWatchMinute->ResumeWatch();
	}
}


void CTab2::OnBnClickedButtonLab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (mThreadStopwatchWork == THREAD_PAUSE) {
		isStopwatchStarted = FALSE;
		StopStopwatch();
	}
	else if (mThreadStopwatchWork == THREAD_RUNNING) {
		LabStopwatch();
	}
}


void CTab2::StartStopwatch() {
	GetDlgItem(IDC_BUTTON_LAB)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("중단"));
	GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("랩"));

	
	mStopwatch = new CustomStopwatch();
	LabStopwatch();

	mThreadStopwatchParam.hwnd = this;
	mThreadStopwatchParam.stopwatch = mStopwatch;	
	mThreadStopwatch = AfxBeginThread(StopwatchThread, &mThreadStopwatchParam);

	mThreadStopwatchWork = THREAD_RUNNING;

	mWatchSecond->StartWatch(mStopwatch);
	mWatchMinute->StartWatch(mStopwatch);
}


void CTab2::StopStopwatch() {
	GetDlgItem(IDC_BUTTON_LAB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("시작"));
	GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("랩"));

	mThreadStopwatch->SuspendThread();
	DWORD dwResult;
	::GetExitCodeThread(mThreadStopwatch->m_hThread, &dwResult);

	delete mThreadStopwatch;
	mThreadStopwatch = NULL;

	delete mStopwatch;
	mStopwatch = NULL;
	mLabList.DeleteAllItems();

	mThreadStopwatchWork = THREAD_STOP;
	Invalidate(TRUE);
	GetDlgItem(IDC_STATIC_STOPWATCH)->SetWindowTextW(_T("00:00.00"));

	mWatchSecond->StopWatch();
	mWatchMinute->StopWatch();
}

void CTab2::OnDraw(CDC* pDC) {

}

void CTab2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	OnDraw(&dc);
	CDialogEx::OnPaint();
}


void CTab2::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CTab2::LabStopwatch()
{
	CString labName;
	labName.Format(_T("랩 %d"), mLabList.GetItemCount() + 1);
	mLabList.InsertItem(0, labName);
	mLabList.SetItemText(0, 1, mStopwatch->GetLabTimeFormatted());
	mStopwatch->SetPrevLabTime(mStopwatch->GetPrevLabTime() + mStopwatch->GetLabTime());
}

void CTab2::AllocateWatch()
{
	CRect rect;
	GetDlgItem(IDC_PC_SECOND)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	mWatchSecond = new CStopwatchWatch;
	mWatchSecond->Create(IDD_DIALOG_WATCH, this);
	mWatchSecond->ShowWindow(SW_SHOW);
	mWatchSecond->MoveWindow(&rect);

	GetDlgItem(IDC_PC_SECOND)->DestroyWindow();

	GetDlgItem(IDC_PC_MINUTE)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	mWatchMinute = new CStopwatchWatch(MODE_MINUTE, 6, 30, 2, 5.0f, 2.5f, 6, 3, 10, 20);
	mWatchMinute->Create(IDD_DIALOG_WATCH, this);
	mWatchMinute->ShowWindow(SW_SHOW);
	mWatchMinute->MoveWindow(&rect);

	GetDlgItem(IDC_PC_MINUTE)->DestroyWindow();
}
