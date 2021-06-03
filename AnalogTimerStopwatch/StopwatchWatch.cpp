// StopwatchWatch.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "StopwatchWatch.h"
#include "afxdialogex.h"


// CStopwatchWatch ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStopwatchWatch, CDialog)

CStopwatchWatch::CStopwatchWatch(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_WATCH, pParent)
{

}

CStopwatchWatch::~CStopwatchWatch()
{
}

void CStopwatchWatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStopwatchWatch, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CStopwatchWatch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CStopwatchWatch::OnDrawWatchImage()
{
	/*
	// Picture Control DC�� ����.
	CClientDC dc(GetDlgItem(IDC_PC_PROGRESS));

	// Picture Control ũ�⸦ ��´�.
	CRect rect;
	GetDlgItem(IDC_PC_PROGRESS)->GetClientRect(&rect);

	// Picture Control�� ȣȯ�Ǵ� Graphics ����.
	Graphics mainG(dc.GetSafeHdc());

	// Picture Control ũ��� ��Ʈ��(�ӽ� ����) ����.
	Bitmap memBmp(rect.Width(), rect.Height());

	// �ӽ� ���ۿ� ȣȯ�Ǵ� Graphics ����.
	Graphics memG(&memBmp);

	memG.Clear(Color::White);

	// �׸��� �۾�.
	Pen DarkGrayPen(Color(255, 38, 37, 44), 3);
	Pen OrangePen(Color(255, 251, 160, 11), 3);
	OrangePen.SetWidth(8.0f);
	DarkGrayPen.SetWidth(7.5f);
	OrangePen.SetStartCap(LineCapRound);
	OrangePen.SetEndCap(LineCapRound);
	memG.SetSmoothingMode(SmoothingModeAntiAlias);
	memG.DrawArc(&DarkGrayPen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, 0, 360);
	memG.DrawArc(&OrangePen, (rect.left + rect.right) / 2 - 160, (rect.top + rect.bottom) / 2 - 160, 320, 320, -90, mProgressSweepAngle);
	// �ӽ� ���۸� Picture Control�� �׸���.
	mainG.DrawImage(&memBmp, 0, 0);
	*/
}

void CStopwatchWatch::DrawWatchOutline(int /*split*/, int /*max*/)
{
	// Picture Control DC�� ����.
	CClientDC dc(GetDlgItem(IDC_PC_HAND));

	// Picture Control ũ�⸦ ��´�.
	CRect rect;
	GetDlgItem(IDC_PC_HAND)->GetClientRect(&rect);

	// Picture Control�� ȣȯ�Ǵ� Graphics ����.
	Graphics mainG(dc.GetSafeHdc());

	// Picture Control ũ��� ��Ʈ��(�ӽ� ����) ����.
	Bitmap memBmp(rect.Width(), rect.Height());

	// �ӽ� ���ۿ� ȣȯ�Ǵ� Graphics ����.
	Graphics memG(&memBmp);

	memG.Clear(Color::Transparent);

	// �׸��� �۾�.
	Pen DarkGrayPen(Color(255, 255 - 88, 255 - 87, 255 - 92));
	Pen BlackPen(Color(255, 0, 0, 0));
	double split = 12;
	double max = 60;
	double unit = max / split;
	double unitSecond = 4;
	int width = 300;
	int height = 300;
	int x = 30;
	int y = 30;
	memG.SetSmoothingMode(SmoothingModeHighQuality);
	float penWidth = 7.0f;
	for (double angle = 0; angle < 360; angle += 360 / unit / split / unitSecond) {
		double j = angle / (360 / split);
		double jj = floor(angle / (360 / split));
		if (angle / (360 / split) == floor(angle / (360 / split))) {
			BlackPen.SetWidth(penWidth * 2);
			memG.DrawArc(&BlackPen, x + penWidth / 2, y + penWidth / 2, width - penWidth, height - penWidth, -90 + angle - 360 / unit / split / unitSecond / 3, 360 / unit / split / unitSecond / 1.5);
		}
		else if (angle / (360 / split / unit) == floor(angle / (360 / split / unit))) {
			DarkGrayPen.SetWidth(penWidth * 2);
			memG.DrawArc(&DarkGrayPen, x + penWidth / 2, y + penWidth / 2, width - penWidth, height - penWidth, -90 + angle - 360 / unit / split / unitSecond / 4, 360 / unit / split / unitSecond / 2);
		}
		else {
			DarkGrayPen.SetWidth(penWidth);
			memG.DrawArc(&DarkGrayPen, x, y, width, height, -90 + angle - 360 / unit / split / unitSecond / 4, 360 / unit / split / unitSecond / 2);
		}
	}

	// �ӽ� ���۸� Picture Control�� �׸���.
	mainG.DrawImage(&memBmp, 0, 0);
}


void CStopwatchWatch::StartWatch()
{

}


void CStopwatchWatch::StopWatch()
{

}


void CStopwatchWatch::ResumeWatch()
{

}


void CStopwatchWatch::PauseWatch()
{

}

void CStopwatchWatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
	OnDraw(&dc);
	CDialog::OnPaint();
}

void CStopwatchWatch::OnDraw(CDC* pDC) {
	DrawWatchOutline(0, 0);
	Graphics graphics(*pDC);
	
}



HBRUSH CStopwatchWatch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CBrush brh;
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	brh.CreateSolidBrush(RGB(255, 255, 255));

	if ((nCtlColor == CTLCOLOR_STATIC) || (nCtlColor == CTLCOLOR_EDIT))
		pDC->SetBkColor(RGB(255, 255, 255));	//���� ����
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(255, 255, 255));	//���� ����
		return brh;
	}
	else
	{
		if (nCtlColor == CTLCOLOR_BTN)
			pDC->SetBkMode(TRANSPARENT);		//��� �����ϰ�
	}
	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}