#pragma once


// CSettingTimerDlg ��ȭ �����Դϴ�.

class CSettingTimerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingTimerDlg)

public:
	CSettingTimerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettingTimerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_TIMER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int mHour;
	int mMinute;
	int mSecond;
};
