#pragma once

#include "CustomStopwatch.h"
#include "afxwin.h"

// CStopwatchWatch 대화 상자입니다.

struct ThreadStopwatchWatchParam {
	CWnd *hwnd;
	CustomStopwatch* stopwatch;
};

class CStopwatchWatch : public CDialog
{
	DECLARE_DYNAMIC(CStopwatchWatch)

public:
	CStopwatchWatch(
		int mode = MODE_SECOND,
		int split = 12,
		int max = 60,
		int subDivision = 4,
		float unitPenWidth = 7.0f,
		float handPenWidth = 3.0f,
		double handDiameter = 6,
		int gapImportance = 1,
		int fontSize = 18,
		double offsetHand = 30,
		CWnd* pParent = NULL
	);
	virtual ~CStopwatchWatch();
	LRESULT StopwatchWatchThreadUpdate(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WATCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	CustomStopwatch* mStopwatch;
	int mMode;
	int mSplit;
	int mMax;
	int mSubDivision;
	float mUnitPenWidth;
	int mGapImportance;
	int mFontSize;
	double mOffsetHand;
	float mSecondHandPenWidth;
	float mHandPenWidth;
	double mHandDiameter;
public:
	virtual BOOL OnInitDialog();
	void OnDrawWatchImage();
	void StartWatch(CustomStopwatch* stopwatch);
	void StopWatch();
	void ResumeWatch();
	void PauseWatch();
	double GetTimeDegree();
	double GetLabTimeDegree();
	afx_msg void OnPaint();
	virtual void OnDraw(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CWinThread* mThreadStopwatchWatch;
	ThreadWorkingType mThreadStopwatchWatchWork;
	ThreadStopwatchWatchParam mThreadStopwatchWatchParam;
};
