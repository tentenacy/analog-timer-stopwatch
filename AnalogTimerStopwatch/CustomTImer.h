#ifndef __CUSTOM_TIMER_H__
#define __CUSTOM_TIMER_H__

#include "CustomTime.h"

class CustomTimer : public CustomTime {
public:
	CustomTimer();
	CustomTimer(int hour, int minute, int second);
	~CustomTimer();
	void TicTok();
	BOOL IsTimeOut();
	void UpdateTime();
	CString GetTimeFormatted() override;
};

#endif