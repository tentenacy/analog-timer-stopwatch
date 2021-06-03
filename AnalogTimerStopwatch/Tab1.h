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

// CTab1 대화 상자입니다.

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab1();
	LRESULT TimerThreadUpdate(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
