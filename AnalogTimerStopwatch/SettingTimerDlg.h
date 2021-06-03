#pragma once


// CSettingTimerDlg 대화 상자입니다.

class CSettingTimerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingTimerDlg)

public:
	CSettingTimerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSettingTimerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_TIMER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int mHour;
	int mMinute;
	int mSecond;
};
