// Tab1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "Tab1.h"
#include "afxdialogex.h"
#include "AnalogTimerStopwatchDlg.h"
#include "SettingTimerDlg.h"

#define IDE_SYSTEM_COUNTER 1000


// CTab1 ��ȭ �����Դϴ�.

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
	ON_WM_TIMER()
END_MESSAGE_MAP()

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
		mProgressSweepAngle = max(360 - 360 * (pParam->time / 1000 / (mHour * 60 * 60 + mMinute * 60 + mSecond)), 0);
		OnDrawProgressImage();
		break;
	case THREAD_UDPATE_TIMEOUT:
		isTimerStarted = FALSE;
		HideTimer();
		mThreadTimerWork = THREAD_STOP;
		AfxMessageBox(_T("Ÿ�̸� �˸�"));
		break;
	}
	
	return 0;
}

// CTab1 �޽��� ó�����Դϴ�.


void CTab1::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	SetTimer(IDE_SYSTEM_COUNTER, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTab1::OnBnClickedButtonStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	isTimerStarted = FALSE;
	StopTimer();
}


void CTab1::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!mHour && !mMinute && !mSecond) return;
	if (!isTimerStarted) {
		isTimerStarted = TRUE;
		StartTimer();
		return;
	}
	if (mThreadTimerWork == THREAD_RUNNING) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("�簳"));
		mThreadTimer->SuspendThread();
		mThreadTimerWork = THREAD_PAUSE;
	}
	else if (mThreadTimerWork == THREAD_PAUSE) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("�Ͻ�����"));
		mThreadTimer->ResumeThread();
		mThreadTimerWork = THREAD_RUNNING;
	}
}


void CTab1::ShowTimer()
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_TIMER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_COUNTER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_HOUR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MINUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HOUR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MINUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SECOND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("�Ͻ�����"));
}


void CTab1::HideTimer()
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_TIMER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_COUNTER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_HOUR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MINUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_HOUR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MINUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SECOND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("����"));
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
}


void CTab1::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(IDE_SYSTEM_COUNTER);
	delete mSystemCounter;
}

void CTab1::OnDraw(CDC* pDC) {
	if (mThreadTimerWork == THREAD_STOP) {
		CClientDC dc(GetDlgItem(IDC_PC_PROGRESS));
		Graphics graphics(dc);
		graphics.Clear(Color::White);
	}
	else if (mThreadTimerWork == THREAD_PAUSE) {
		OnDrawProgressImage();
	}
}


void CTab1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	OnDraw(&dc);
	CDialogEx::OnPaint();
}


HBRUSH CTab1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CBrush brh;
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	brh.CreateSolidBrush(RGB(255, 255, 255));

	if ((nCtlColor == CTLCOLOR_STATIC) || (nCtlColor == CTLCOLOR_EDIT))
		pDC->SetBkColor(RGB(255, 255, 255));	//���� ����
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(255, 255, 255));	//���� ����
		return brh;
	}
	else
	{
		if (nCtlColor == CTLCOLOR_BTN)
			pDC->SetBkMode(TRANSPARENT);		//��� �����ϰ�
	}
	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}


void CTab1::OnDrawProgressImage()
{
	// Picture Control DC�� ����.
	CClientDC dc(GetDlgItem(IDC_PC_PROGRESS));	

	// Picture Control ũ�⸦ ��´�.
	CRect rect;
	GetDlgItem(IDC_PC_PROGRESS)->GetClientRect(&rect);

	// Picture Control�� ȣȯ�Ǵ� Graphics ����.
	Graphics mainG(dc.GetSafeHdc());

	// Picture Control ũ��� ��Ʈ��(�ӽ� ����) ����.
	Bitmap memBmp(rect.Width(), rect.Height());

	// �ӽ� ���ۿ� ȣȯ�Ǵ� Graphics ����.
	Graphics memG(&memBmp);

	memG.Clear(Color::White);

	// �׸��� �۾�.
	Pen DarkGrayPen(Color(255, 38, 37, 44));
	Pen OrangePen(Color(255, 251, 160, 11));
	OrangePen.SetWidth(8.0f);
	DarkGrayPen.SetWidth(7.5f);
	OrangePen.SetStartCap(LineCapRound);
	OrangePen.SetEndCap(LineCapRound);
	memG.SetSmoothingMode(SmoothingModeAntiAlias);
	memG.DrawArc(&DarkGrayPen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, 0, 360);
	memG.DrawArc(&OrangePen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, -90, mProgressSweepAngle);
	// �ӽ� ���۸� Picture Control�� �׸���.
	mainG.DrawImage(&memBmp, 0, 0);
}

void CTab1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent) {
	case IDE_SYSTEM_COUNTER:
		mSystemCounter->TicTok();
		GetDlgItem(IDC_STATIC_COUNTER)->SetWindowTextW(mSystemCounter->GetTimeFormatted());
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}