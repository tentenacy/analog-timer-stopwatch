
// AnalogTimerStopwatch.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CAnalogTimerStopwatchApp:
// �� Ŭ������ ������ ���ؼ��� AnalogTimerStopwatch.cpp�� �����Ͻʽÿ�.
//

class CAnalogTimerStopwatchApp : public CWinApp
{
public:
	CAnalogTimerStopwatchApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAnalogTimerStopwatchApp theApp;