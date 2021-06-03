#include "stdafx.h"
#include "CustomStopwatch.h"

CustomStopwatch::CustomStopwatch() : 
	CustomTime(0, 0, 0), 
	mMilliSecond(0), 
	mLabHour(0), 
	mLabMinute(0), 
	mLabSecond(0), 
	mLabMilliSecond(0),
	mPrevLabTime(0)
{
}

CustomStopwatch::~CustomStopwatch()
{
}

void CustomStopwatch::SetTime(int time) {
	mMilliSecond = time % 1000;
	mSecond = (time / 1000) % 60;
	mMinute = (time / 1000 / 60) % 60;
	mHour = time / 1000 / 60 / 60;
}

void CustomStopwatch::SetLabTime(int time) {
	mLabMilliSecond = (time - mPrevLabTime) % 1000;
	mLabSecond = ((time - mPrevLabTime) / 1000) % 60;
	mLabMinute = ((time - mPrevLabTime) / 1000 / 60) % 60;
	mLabHour = (time - mPrevLabTime) / 1000 / 60 / 60;
}

CString CustomStopwatch::GetTimeFormatted() {
	CString ret;
	if (mHour > 0) {
		ret.Format(_T("%d:%02d:%02d.%02d"), mHour, mMinute, mSecond, mMilliSecond / 10);
	}
	else {
		ret.Format(_T("%02d:%02d.%02d"), mMinute, mSecond, mMilliSecond / 10);
	}
	return ret;
}

CString CustomStopwatch::GetLabTimeFormatted() {
	CString ret;
	if (mLabHour > 0) {
		ret.Format(_T("%d:%02d:%02d.%02d"), mLabHour, mLabMinute, mLabSecond, mLabMilliSecond / 10);
	}
	else {
		ret.Format(_T("%02d:%02d.%02d"), mLabMinute, mLabSecond, mLabMilliSecond / 10);
	}
	return ret;
}

int CustomStopwatch::GetMilliSecond() {
	return mMilliSecond;
}

int CustomStopwatch::GetTime() {
	return mHour * 60 * 60 * 1000 + mMinute * 60 * 1000 + mSecond * 1000 + mMilliSecond;
}

int CustomStopwatch::GetLabTime() {
	return mLabHour * 60 * 60 * 1000 + mLabMinute * 60 * 1000 + mLabSecond * 1000 + mLabMilliSecond;
}

void CustomStopwatch::SetPrevLabTime(int time) {
	mPrevLabTime = time;
}

int CustomStopwatch::GetPrevLabTime() {
	return mPrevLabTime;
}