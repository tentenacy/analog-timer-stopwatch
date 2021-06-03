#pragma once
#include "afxwin.h"
#include "CustomStopwatch.h"
#include "StopwatchWatch.h"

#define THREAD_STOPWATCH WM_USER + (1 << 1)

enum ThreadStopwatchUpdateParamType
{
	THREAD_UPDATE_STOPWATCH = 1,
	THREAD_UPDATE_LAB
};

struct ThreadStopwatchUpdateParam {
	CustomStopwatch* stopwatch;
	double time;
};

struct ThreadStopwatchParam {
	CWnd *hwnd;
	CustomStopwatch* stopwatch;
};

// CTab2 대화 상자입니다.

class CTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2)

public:
	CTab2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab2();
	LRESULT StopwatchThreadUpdate(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnDraw(CDC* pDC);
public:
	virtual BOOL OnInitDialog();
	CFont mFont;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonLab();

	void StartStopwatch();
	void StopStopwatch();
	void LabStopwatch();
	BOOL isStopwatchStarted;

	CWinThread* mThreadStopwatch;
	ThreadStopwatchParam mThreadStopwatchParam;
	ThreadWorkingType mThreadStopwatchWork;
	afx_msg void OnPaint();
	afx_msg void OnDestroy();


	CListCtrl mLabList;
	CustomStopwatch *mStopwatch;
	void AllocateWatch();
	CStopwatchWatch* mWatchSecond;
};
