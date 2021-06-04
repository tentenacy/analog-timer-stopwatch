// StopwatchWatch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnalogTimerStopwatch.h"
#include "StopwatchWatch.h"
#include "afxdialogex.h"

#define PI 3.14159265

// CStopwatchWatch 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStopwatchWatch, CDialog)

CStopwatchWatch::CStopwatchWatch(
	int mode,
	int split,
	int max,
	int subDivision,
	float unitPenWidth,
	float handPenWidth,
	double handDiameter,
	int gapImportance,
	int fontHeight,
	double offsetHand,
	CWnd* pParent /*=NULL*/
)	: CDialog(IDD_DIALOG_WATCH, pParent),
	mMode(mode),
	mSplit(split),
	mMax(max),
	mSubDivision(subDivision),
	mUnitPenWidth(unitPenWidth),
	mHandPenWidth(handPenWidth),
	mHandDiameter(handDiameter),
	mGapImportance(gapImportance),
	mFontSize(fontHeight),
	mOffsetHand(offsetHand),
	mThreadStopwatchWatch(NULL),
	mStopwatch(NULL)
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
	ON_MESSAGE(THREAD_STOPWATCH_WATCH, StopwatchWatchThreadUpdate)
END_MESSAGE_MAP()

UINT StopwatchWatchThread(LPVOID param)
{
	ThreadStopwatchWatchParam* pParam = (ThreadStopwatchWatchParam*)param;
	CStopwatchWatch* pMain = (CStopwatchWatch*)pParam->hwnd;
	CustomStopwatch* stopwatch = pParam->stopwatch;

	for (; pMain->mThreadStopwatchWatchWork == THREAD_RUNNING; ) {
		::PostMessage(pMain->m_hWnd, THREAD_STOPWATCH_WATCH, NULL, NULL);
		Sleep(2);
	}
	return 0;
}

LRESULT CStopwatchWatch::StopwatchWatchThreadUpdate(WPARAM wParam, LPARAM lParam) {
	OnDrawWatchImage();
	return 0;
}

BOOL CStopwatchWatch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStopwatchWatch::OnDrawWatchImage()
{
	// Picture Control DC를 생성.
	CClientDC dc(this);

	// Picture Control 크기를 얻는다.
	CRect rect;
	GetClientRect(&rect);

	// Picture Control에 호환되는 Graphics 생성.
	Graphics mainG(dc.GetSafeHdc());

	// Picture Control 크기로 비트맵(임시 버퍼) 생성.
	Bitmap memBmp(rect.Width(), rect.Height());

	// 임시 버퍼에 호환되는 Graphics 생성.
	Graphics memG(&memBmp);

	memG.Clear(Color::White);

	int unit = mMax / mSplit;
	int diameter = min(rect.Width(), rect.Height()) * 0.9;
	int x = rect.Width() / 2.0 - diameter / 2.0;
	int y = rect.Height() / 2.0 - diameter / 2.0;
	int beginAngle = -90;

	CString strTemp;

	FontFamily fontFamily(L"MS Shell Dlg");
	Gdiplus::Font font(&fontFamily, mFontSize, FontStyleRegular, UnitPoint);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetTrimming(StringTrimmingNone);
	SolidBrush brush(Color::Black);

	// 그리는 작업.
	Pen DarkGrayPen(Color(255, 255 - 88, 255 - 87, 255 - 92));
	Pen BlackPen(Color(255, 0, 0, 0));
	memG.SetSmoothingMode(SmoothingModeHighQuality);
	for (double angle = 360.0 / mSplit; angle < 360.0 / mSplit + 360; angle += 360.0 / unit / mSplit / mSubDivision) {
		if (angle / (360.0 / mSplit) == floor(angle / (360.0 / mSplit))) {
			BlackPen.SetWidth(mUnitPenWidth * 2);
			memG.DrawArc(
				&BlackPen,
				x + mUnitPenWidth / 2,
				y + mUnitPenWidth / 2,
				diameter - mUnitPenWidth,
				diameter - mUnitPenWidth,
				beginAngle + angle - 360.0 / unit / mSplit / mSubDivision * ((0.5 * 1.5) / (0.5 + 0.5 * 1.5 + mGapImportance)),
				360.0 / unit / mSplit / mSubDivision * (2 * (0.5 * 1.5) / (0.5 + 0.5 * 1.5 + mGapImportance))
			);
			strTemp.Format(_T("%d"), unit * (int)floor(angle / (360.0 / mSplit)));
			memG.DrawString(
				strTemp,
				-1,
				&font,
				RectF(
					PointF(rect.CenterPoint().x, rect.CenterPoint().y) +
					PointF(cos((90 - angle) * PI / 180)*(diameter / 2.0 - mOffsetHand), -sin((90 - angle) * PI / 180)*(diameter / 2.0 - mOffsetHand)) +
					PointF(-mFontSize*strTemp.GetLength()/2, -(mFontSize*1.5) / 2.0),
					SizeF(
						mFontSize*strTemp.GetLength(),
						mFontSize*1.5
					)
				),
				&stringFormat,
				&brush
			);
		}
		else if (angle / (360.0 / mSplit / unit) == floor(angle / (360.0 / mSplit / unit))) {
			DarkGrayPen.SetWidth(mUnitPenWidth * 2);
			memG.DrawArc(
				&DarkGrayPen,
				x + mUnitPenWidth / 2,
				y + mUnitPenWidth / 2,
				diameter - mUnitPenWidth,
				diameter - mUnitPenWidth,
				beginAngle + angle - 360.0 / unit / mSplit / mSubDivision * (0.5 / (1 + mGapImportance)),
				360.0 / unit / mSplit / mSubDivision * (2 * 0.5 / (1 + mGapImportance))
			);
		}
		else {
			DarkGrayPen.SetWidth(mUnitPenWidth);
			memG.DrawArc(
				&DarkGrayPen,
				x,
				y,
				diameter,
				diameter,
				beginAngle + angle - 360.0 / unit / mSplit / mSubDivision * (0.5 / (1 + mGapImportance)),
				360.0 / unit / mSplit / mSubDivision * (2 * 0.5 / (1 + mGapImportance))
			);
		}
	}

	if (mStopwatch != NULL && mMode == MODE_SECOND && mStopwatch->GetPrevLabTime() != 0) {
		// 그리는 작업.
		Pen BluePen(Color(255, 27, 80, 148));
		// BluePen.SetWidth(2 * diameter / 2.0 * sin((360.0 / unit / mSplit / mSubDivision * (2 * (0.5 * 1.5) / (0.5 + 0.5 * 1.5 + mGapImportance)) / 2) * PI / 180));
		BluePen.SetWidth(mHandPenWidth);
		memG.SetSmoothingMode(SmoothingModeAntiAlias);
		memG.DrawEllipse(
			&BluePen,
			Rect(
				Point(rect.CenterPoint().x, rect.CenterPoint().y) +
				Point(-mHandDiameter / 2.0, -mHandDiameter / 2.0),
				Size(mHandDiameter, mHandDiameter)
			)
		);
		memG.DrawLine(
			&BluePen,
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetLabTimeDegree())*(-diameter / 8.0),
				-sin(GetLabTimeDegree())*(-diameter / 8.0)
			),
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetLabTimeDegree())*(-mHandDiameter / 2.0),
				-sin(GetLabTimeDegree())*(-mHandDiameter / 2.0)
			)
		);
		memG.DrawLine(
			&BluePen,
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetLabTimeDegree())*(mHandDiameter / 2.0),
				-sin(GetLabTimeDegree())*(mHandDiameter / 2.0)
			),
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetLabTimeDegree())*(diameter / 2.0 + mUnitPenWidth - BluePen.GetWidth()),
				-sin(GetLabTimeDegree())*(diameter / 2.0 + mUnitPenWidth - BluePen.GetWidth())
			)
		);
	}

	// 그리는 작업.
	Pen OrangePen(Color(255, 251, 160, 11));
	// OrangePen.SetWidth(2 * diameter / 2.0 * sin((360.0 / unit / mSplit / mSubDivision * (2 * (0.5 * 1.5) / (0.5 + 0.5 * 1.5 + mGapImportance)) / 2) * PI / 180));
	OrangePen.SetWidth(mHandPenWidth);
	if (mMode == MODE_SECOND) {
		memG.DrawEllipse(
			&OrangePen,
			Rect(
				Point(rect.CenterPoint().x, rect.CenterPoint().y) +
				Point(-mHandDiameter / 2.0, -mHandDiameter / 2.0),
				Size(mHandDiameter, mHandDiameter)
			)
		);
		memG.DrawLine(
			&OrangePen,
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetTimeDegree())*(-diameter / 8.0),
				-sin(GetTimeDegree())*(-diameter / 8.0)
			),
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetTimeDegree())*(-mHandDiameter / 2.0),
				-sin(GetTimeDegree())*(-mHandDiameter / 2.0)
			)
		);
		memG.DrawLine(
			&OrangePen,
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetTimeDegree())*(mHandDiameter / 2.0),
				-sin(GetTimeDegree())*(mHandDiameter / 2.0)
			),
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetTimeDegree())*(diameter / 2.0 + mUnitPenWidth - OrangePen.GetWidth()),
				-sin(GetTimeDegree())*(diameter / 2.0 + mUnitPenWidth - OrangePen.GetWidth())
			)
		);
	}
	else if (mMode == MODE_MINUTE) {
		OrangePen.SetStartCap(LineCapRoundAnchor);
		memG.DrawLine(
			&OrangePen,
			Point(rect.CenterPoint().x, rect.CenterPoint().y),
			Point(rect.CenterPoint().x, rect.CenterPoint().y) +
			Point(
				cos(GetTimeDegree())*(diameter / 2.0 + mUnitPenWidth - OrangePen.GetWidth()),
				-sin(GetTimeDegree())*(diameter / 2.0 + mUnitPenWidth - OrangePen.GetWidth())
			)
		);
	}

	// 임시 버퍼를 Picture Control에 그리기.
	mainG.DrawImage(&memBmp, 0, 0);
}

void CStopwatchWatch::StartWatch(CustomStopwatch* stopwatch)
{
	mStopwatch = stopwatch;

	mThreadStopwatchWatchParam.hwnd = this;
	mThreadStopwatchWatchParam.stopwatch = mStopwatch;
	mThreadStopwatchWatch = AfxBeginThread(StopwatchWatchThread, &mThreadStopwatchWatchParam);

	mThreadStopwatchWatchWork = THREAD_RUNNING;
}


void CStopwatchWatch::StopWatch()
{
	mThreadStopwatchWatch->SuspendThread();
	DWORD dwResult;
	::GetExitCodeThread(mThreadStopwatchWatch->m_hThread, &dwResult);

	delete mThreadStopwatchWatch;
	mThreadStopwatchWatch = NULL;

	mStopwatch = NULL;

	mThreadStopwatchWatchWork = THREAD_STOP;
	Invalidate(TRUE);
}


void CStopwatchWatch::ResumeWatch()
{
	mThreadStopwatchWatch->ResumeThread();
}


void CStopwatchWatch::PauseWatch()
{
	mThreadStopwatchWatch->SuspendThread();
}

void CStopwatchWatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	OnDraw(&dc);
	CDialog::OnPaint();
}

void CStopwatchWatch::OnDraw(CDC* pDC) {
	OnDrawWatchImage();
}

HBRUSH CStopwatchWatch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CBrush brh;
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	brh.CreateSolidBrush(RGB(255, 255, 255));

	if ((nCtlColor == CTLCOLOR_STATIC) || (nCtlColor == CTLCOLOR_EDIT))
		pDC->SetBkColor(RGB(255, 255, 255));	//배경색 변경
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(255, 255, 255));	//배경색 변경
		return brh;
	}
	else
	{
		if (nCtlColor == CTLCOLOR_BTN)
			pDC->SetBkMode(TRANSPARENT);		//배경 투명하게
	}
	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}

double CStopwatchWatch::GetTimeDegree() {
	if (mStopwatch == NULL) {
		return 90 * PI / 180;
	}
	switch (mMode) {
	case MODE_SECOND:
		return (90 - 360 * ((float)((mStopwatch->GetTime() % (1000 * 60))) / mMax / 1000)) * PI / 180;
	case MODE_MINUTE:
		return (90 - 360 * ((float)((mStopwatch->GetTime() % (1000 * 60 * 60))) / mMax / 1000 / 60)) * PI / 180;
	}
}

double CStopwatchWatch::GetLabTimeDegree() {
	switch (mMode) {
	case MODE_SECOND:
		return (90 - 360 * ((float)((mStopwatch->GetLabTime() % (1000 * 60))) / mMax / 1000)) * PI / 180;
	case MODE_MINUTE:
		return (90 - 360 * ((float)((mStopwatch->GetLabTime() % (1000 * 60 * 60))) / mMax / 1000)) * PI / 180;
	}
}