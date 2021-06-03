#pragma once


// CStopwatchWatch 대화 상자입니다.

class CStopwatchWatch : public CDialog
{
	DECLARE_DYNAMIC(CStopwatchWatch)

public:
	CStopwatchWatch(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStopwatchWatch();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WATCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnDrawWatchImage();
	void StartWatch();
	void StopWatch();
	void ResumeWatch();
	void PauseWatch();
	void DrawWatchOutline(int split, int max);
	afx_msg void OnPaint();
	virtual void OnDraw(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
