#ifndef __CUSTOM_STOPWATCH_H__
#define __CUSTOM_STOPWATCH_H__

#include "CustomTime.h"

class CustomStopwatch: public CustomTime {
protected:
	int mMilliSecond;
	int mLabHour;
	int mLabMinute;
	int mLabSecond;
	int mLabMilliSecond;
	int mPrevLabTime;

public:
	CustomStopwatch();
	~CustomStopwatch();
	void SetTime(int time);
	CString GetTimeFormatted() override;
	int GetMilliSecond();
	int GetTime();
	int GetLabTime();
	void SetLabTime(int time);
	CString GetLabTimeFormatted();
	void SetPrevLabTime(int time);
	int GetPrevLabTime();
};

#endif
