#include "stdafx.h"
#include "CustomTimer.h"

CustomTimer::CustomTimer() : CustomTime()
{
}

CustomTimer::CustomTimer(int hour, int minute, int second) : CustomTime(hour, minute, second)
{
}

CustomTimer::~CustomTimer()
{
}

void CustomTimer::TicTok() {
	mSecond -= 1;
	UpdateTime();
}

void CustomTimer::UpdateTime() {
	if (mSecond < 0) {
		mMinute -= 1;
		mSecond = 59;
	}

	if (mMinute < 0) {
		mHour -= 1;
		mMinute = 59;
	}

	if (mHour < 0) {
		mHour = 0;
		mMinute = 0;
		mSecond = 0;
	}
}

BOOL CustomTimer::IsTimeOut() {
	return !mHour && !mMinute && !mSecond;
}

CString CustomTimer::GetTimeFormatted() {
	CString ret;
	if (this->mHour) {
		ret.Format(_T("%2d:%02d:%02d"), mHour, mMinute, mSecond);
	}
	else {
		ret.Format(_T("  %02d:%02d  "), mMinute, mSecond);
	}
	return ret;
}