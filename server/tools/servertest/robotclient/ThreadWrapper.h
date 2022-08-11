#pragma once

#include "TestCase.h"

class ThreadWrapper
{
public:
	ThreadWrapper();
	virtual ~ThreadWrapper(void);

	void set_thread( int startnum, int num, int range);

public:
	void start_thread();
	void stop_thread();

	void set_cancel();

	static DWORD  WINAPI Thread(LPVOID  lparam);
	void main_loop();

private:
	TestCase	*cases_;
	int			case_num_;

	bool	cancel_;
	HANDLE	hthread;
	DWORD	dwThreadId;
};
