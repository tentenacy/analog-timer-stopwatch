#ifndef __CUSTOM_TIME_H__
#define __CUSTOM_TIME_H__

class CustomTime {
protected:
	int mHour;
	int mMinute;
	int mSecond;
public:
	CustomTime();
	CustomTime(int hour, int minute, int second);
	~CustomTime();
	virtual CString GetTimeFormatted();
	int GetHour();
	int GetMinute();
	int GetSecond();
	void SetHour(int hour);
	void SetMinute(int minute);
	void SetSecond(int second);
};

#endif
