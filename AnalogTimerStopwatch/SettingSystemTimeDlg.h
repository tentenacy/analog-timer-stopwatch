#pragma once


// CSettingSystemTimeDlg ��ȭ �����Դϴ�.

class CSettingSystemTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingSystemTimeDlg)

public:
	CSettingSystemTimeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettingSystemTimeDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_SYSTEM_TIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int mHour;
	int mMinute;
	int mSecond;
	afx_msg void OnBnClickedOk();
};
