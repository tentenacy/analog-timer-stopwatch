#pragma once


// CStopwatchWatch ��ȭ �����Դϴ�.

class CStopwatchWatch : public CDialog
{
	DECLARE_DYNAMIC(CStopwatchWatch)

public:
	CStopwatchWatch(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStopwatchWatch();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WATCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
