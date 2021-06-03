#include "stdafx.h"
#include "CustomSystemCounter.h"

CustomSystemCounter::CustomSystemCounter() : CustomTime()
{
}

CustomSystemCounter::CustomSystemCounter(int hour, int minute, int second) : CustomTime(hour, minute, second)
{
}

CustomSystemCounter::~CustomSystemCounter()
{
}

void CustomSystemCounter::TicTok() {
	mSecond += 1;
	UpdateTime();
}

void CustomSystemCounter::UpdateTime() {
	if (mSecond > 59) {
		mMinute += 1;
		mSecond = 0;
	}

	if (mMinute > 59) {
		mHour += 1;
		mMinute = 0;
	}

	if (mHour > 23) {
		mHour = 0;
	}
}

CString CustomSystemCounter::GetTimeFormatted() {
	CString ret;
	if(mHour < 12) {
		ret.Format(_T("오전 %d:%02d:%02d"), mHour , mMinute, mSecond);
	}
	else if (mHour == 12) {
		ret.Format(_T("오후 %d:%02d:%02d"), mHour, mMinute, mSecond);
	}
	else if (mHour > 12) {
		ret.Format(_T("오후 %d:%02d:%02d"), mHour-12, mMinute, mSecond);
	}
	
	return ret;
}