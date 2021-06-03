#ifndef __CUSTOM_SYSTEM_CLOCK_H__
#define __CUSTOM_SYSTEM_CLOCK_H__

#include "CustomTime.h"

class CustomSystemCounter : public CustomTime {
public:
	CustomSystemCounter();
	CustomSystemCounter(int hour, int minute, int second);
	~CustomSystemCounter();
	void TicTok();
	void UpdateTime();
	CString GetTimeFormatted() override;
};

#endif
