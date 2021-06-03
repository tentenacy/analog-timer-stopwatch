#include "stdafx.h"
#include "CustomTime.h"

CustomTime::CustomTime()
{
	COleDateTime oleDateTime = COleDateTime::GetCurrentTime();
	mHour = oleDateTime.GetHour();
	mMinute = oleDateTime.GetMinute();
	mSecond = oleDateTime.GetSecond();
}

CustomTime::CustomTime(int hour, int minute, int second) : mHour(hour), mMinute(minute), mSecond(second)
{
}

CustomTime::~CustomTime()
{

}

int CustomTime::GetHour() {
	return mHour;
}

int CustomTime::GetMinute() {
	return mMinute;
}

int CustomTime::GetSecond() {
	return mSecond;
}

CString CustomTime::GetTimeFormatted() {
	CString ret;
	ret.Format(_T("%d:%02d:%02d"), mHour, mMinute, mSecond);
	return ret;
}