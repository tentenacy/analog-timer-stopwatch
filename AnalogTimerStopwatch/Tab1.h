#pragma once
#include "afxwin.h"
#include "CustomTImer.h"
#include "CustomSystemCounter.h"

#define THREAD_TIMER WM_USER

enum ThreadTimerUpdateParamType
{
	THREAD_UPDATE_TIMER = 1,
	THREAD_UPDATE_PROGRESS,
	THREAD_UDPATE_TIMEOUT
};

struct ThreadTimerUpdateParam {
	double time;
};

struct ThreadTimerParam {
	CWnd *hwnd;
	CustomTimer* timer;
};

// CTab1 ��ȭ �����Դϴ�.

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab1();
	LRESULT TimerThreadUpdate(WPARAM wParam, LPARAM lParam);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnDraw(CDC* pDC);
	
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
public:
	CFont mFont;
	int mHour;
	int mMinute;
	int mSecond;
	
	void ShowTimer();
	void HideTimer();
	void StartTimer();
	void StopTimer();
	
	CustomSystemCounter* mSystemCounter;
	BOOL isTimerStarted;
	CustomTimer* mTimer;
	
	CWinThread* mThreadTimer;
	ThreadWorkingType mThreadTimerWork;
	ThreadTimerParam mThreadTimerParam;

	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStart();
	void OnDrawProgressImage();
	double mProgressSweepAngle;
};
