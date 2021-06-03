// Tab2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "Tab2.h"
#include "afxdialogex.h"
#include "CustomStopwatch.h"


// CTab2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTab2, CDialogEx)

CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, isStopwatchStarted(FALSE)
	, mThreadStopwatch(NULL)
	, mStopwatch(NULL)
	, mWatchSecond(NULL)
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

// CTab2 �޽��� ó�����Դϴ�.


void CTab2::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CTab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	mLabList.InsertColumn(0, _T("��"), LVCFMT_LEFT, 260);
	mLabList.InsertColumn(1, _T("���"), LVCFMT_LEFT, width - 260);

	AllocateWatch();

	mWatchSecond->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_STOPWATCH)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_BUTTON_START)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_BUTTON_LAB)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	GetDlgItem(IDC_STATIC_STOPWATCH)->BringWindowToTop();
	GetDlgItem(IDC_BUTTON_START)->BringWindowToTop();
	GetDlgItem(IDC_BUTTON_LAB)->BringWindowToTop();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

HBRUSH CTab2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CTab2::OnBnClickedButtonStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!isStopwatchStarted) {
		isStopwatchStarted = TRUE;
		StartStopwatch();
		return;
	}

	if (mThreadStopwatchWork == THREAD_RUNNING) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("����"));
		GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("�缳��"));
		mThreadStopwatch->SuspendThread();
		mThreadStopwatchWork = THREAD_PAUSE;
	}
	else if (mThreadStopwatchWork == THREAD_PAUSE) {
		GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("�ߴ�"));
		GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("��"));
		mThreadStopwatch->ResumeThread();
		mThreadStopwatchWork = THREAD_RUNNING;
	}
}


void CTab2::OnBnClickedButtonLab()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("�ߴ�"));
	GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("��"));

	
	mStopwatch = new CustomStopwatch();
	LabStopwatch();

	mThreadStopwatchParam.hwnd = this;
	mThreadStopwatchParam.stopwatch = mStopwatch;	
	mThreadStopwatch = AfxBeginThread(StopwatchThread, &mThreadStopwatchParam);

	mThreadStopwatchWork = THREAD_RUNNING;
}


void CTab2::StopStopwatch() {
	GetDlgItem(IDC_BUTTON_LAB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowTextW(_T("����"));
	GetDlgItem(IDC_BUTTON_LAB)->SetWindowTextW(_T("��"));

	mThreadStopwatch->SuspendThread();
	DWORD dwResult;
	::GetExitCodeThread(mThreadStopwatch->m_hThread, &dwResult);

	delete mThreadStopwatch;
	mThreadStopwatch = NULL;

	delete mStopwatch;
	mLabList.DeleteAllItems();

	mThreadStopwatchWork = THREAD_STOP;
	Invalidate(TRUE);
	GetDlgItem(IDC_STATIC_STOPWATCH)->SetWindowTextW(_T("00:00.00"));
}

void CTab2::OnDraw(CDC* pDC) {

}

void CTab2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	OnDraw(&dc);
	CDialogEx::OnPaint();
}


void CTab2::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CTab2::LabStopwatch()
{
	CString labName;
	labName.Format(_T("�� %d"), mLabList.GetItemCount() + 1);
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
}
