
// AnalogTimerStopwatchDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "SettingTimerDlg.h"

class CTab1;
class CTab2;

// CAnalogTimerStopwatchDlg ��ȭ ����
class CAnalogTimerStopwatchDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CAnalogTimerStopwatchDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	CTab1 *mDlg1;
	CTab2 *mDlg2;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALOGTIMERSTOPWATCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
