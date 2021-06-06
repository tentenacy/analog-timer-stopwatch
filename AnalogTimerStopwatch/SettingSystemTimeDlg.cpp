// SettingSystemTimeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "SettingSystemTimeDlg.h"
#include "afxdialogex.h"
#include "AnalogTimerStopwatchDlg.h"
#include "Tab1.h"


// CSettingSystemTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSettingSystemTimeDlg, CDialogEx)

CSettingSystemTimeDlg::CSettingSystemTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_SYSTEM_TIME, pParent)
	, mHour(0)
	, mMinute(0)
	, mSecond(0)
{

}

CSettingSystemTimeDlg::~CSettingSystemTimeDlg()
{
}

void CSettingSystemTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOUR, mHour);
	DDV_MinMaxInt(pDX, mHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mMinute);
	DDV_MinMaxInt(pDX, mMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECOND, mSecond);
	DDV_MinMaxInt(pDX, mSecond, 0, 59);
}


BEGIN_MESSAGE_MAP(CSettingSystemTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingSystemTimeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingSystemTimeDlg 메시지 처리기입니다.


void CSettingSystemTimeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CAnalogTimerStopwatchDlg* pParent = (CAnalogTimerStopwatchDlg*)GetParent();
	if (UpdateData(TRUE)) {
		pParent->mDlg1->mSystemCounter->SetHour(mHour);
		pParent->mDlg1->mSystemCounter->SetMinute(mMinute);
		pParent->mDlg1->mSystemCounter->SetSecond(mSecond);
	}

	CDialogEx::OnOK();
}
