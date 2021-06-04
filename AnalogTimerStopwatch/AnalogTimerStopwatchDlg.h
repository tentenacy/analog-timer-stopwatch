
// AnalogTimerStopwatchDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "SettingTimerDlg.h"

class CTab1;
class CTab2;

// CAnalogTimerStopwatchDlg 대화 상자
class CAnalogTimerStopwatchDlg : public CDialogEx
{
// 생성입니다.
public:
	CAnalogTimerStopwatchDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	CTab1 *mDlg1;
	CTab2 *mDlg2;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALOGTIMERSTOPWATCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl mTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32771();
	BOOL SetHook();
	BOOL UnHook();
	virtual BOOL DestroyWindow();
};
