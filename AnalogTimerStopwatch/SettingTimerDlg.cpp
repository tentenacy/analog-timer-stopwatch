// SettingTimer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "SettingTimerDlg.h"
#include "afxdialogex.h"

#include "AnalogTimerStopwatchDlg.h"
#include "Tab1.h"


// CSettingTimerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSettingTimerDlg, CDialogEx)

CSettingTimerDlg::CSettingTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_TIMER, pParent)
	, mHour(0)
	, mMinute(0)
	, mSecond(0)
{

}

CSettingTimerDlg::~CSettingTimerDlg()
{
}

void CSettingTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUR, mHour);
	DDV_MinMaxInt(pDX, mHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mMinute);
	DDV_MinMaxInt(pDX, mMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECOND, mSecond);
	DDV_MinMaxInt(pDX, mSecond, 0, 59);
}


BEGIN_MESSAGE_MAP(CSettingTimerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingTimerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingTimerDlg �޽��� ó�����Դϴ�.


void CSettingTimerDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CAnalogTimerStopwatchDlg* pParent = (CAnalogTimerStopwatchDlg*)GetParent();
	if (UpdateData(TRUE)) {
		pParent->mDlg1->mHour = mHour;
		pParent->mDlg1->mMinute = mMinute;
		pParent->mDlg1->mSecond = mSecond;
	}
	pParent->mDlg1->UpdateData(FALSE);

	CDialogEx::OnOK();
}
