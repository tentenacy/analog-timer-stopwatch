#pragma once
#include "afxwin.h"
#include "CustomStopwatch.h"
#include "StopwatchWatch.h"

enum ThreadStopwatchUpdateParamType
{
	THREAD_UPDATE_STOPWATCH = 1,
	THREAD_UPDATE_LAB
};

struct ThreadStopwatchParam {
	CWnd *hwnd;
	CustomStopwatch* stopwatch;
};

// CTab2 ��ȭ �����Դϴ�.

class CTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2)

public:
	CTab2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab2();
	LRESULT StopwatchThreadUpdate(WPARAM wParam, LPARAM lParam);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	CStopwatchWatch* mWatchMinute;
};
