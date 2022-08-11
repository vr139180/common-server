#include "StdAfx.h"

#include "threadwrapper.h"
#include <cmsLib/Log.h>
#include "testcase/core/TestCaseWorkFlow.h"

extern CString		g_ip;
extern int			g_port;

ThreadWrapper::ThreadWrapper()
:cases_( 0),
cancel_( false),
hthread( 0),
dwThreadId( 0),
case_num_( 0)
{
}

ThreadWrapper::~ThreadWrapper(void)
{
	/*
	if( !cancel_)
		stop_thread();
	*/
	stop_thread();
}

void ThreadWrapper::set_thread( int startnum, int num, int range)
{
	case_num_ =num;

	if( case_num_ > 0)
	{
		int uid =startnum;
		cases_ =new TestCase[case_num_];
		for( int i =0; i < case_num_; ++i)
		{
			cases_[i].set_userrange( uid, range, 0);
			cases_[i].set_ip( (LPCTSTR)g_ip, g_port);

			uid += range;
		}
	}
}

DWORD  WINAPI ThreadWrapper::Thread(LPVOID  lparam)  
{  
	ThreadWrapper *pThis =reinterpret_cast<ThreadWrapper*>(lparam);

	srand(static_cast<int>(time( 0)));

	logInfo(out_runtime, "start thread: %d", pThis->dwThreadId);

	pThis->main_loop();

	logInfo(out_runtime, "end thread: %d", pThis->dwThreadId);

	return 1;
}

void ThreadWrapper::start_thread()
{
	if( hthread)
		return;

	hthread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Thread, this,0,&dwThreadId);  
}

void ThreadWrapper::stop_thread()
{
	if( cases_)
	{
		for( int i =0; i < case_num_; ++i)
			cases_[i].set_cancel();
	}

	cancel_ =true;
	if( hthread > 0)
	{
		::WaitForSingleObject( hthread, INFINITE);
		CloseHandle( hthread);
	}

	hthread =0;
	dwThreadId =0;

	if( cases_)
		delete[] cases_;
	cases_ =0;
}

void ThreadWrapper::set_cancel()
{ 
	cancel_ =true; 
	if( cases_)
	{
		for( int i =0; i < case_num_; ++i)
			cases_[i].set_cancel();
	}
};

void ThreadWrapper::main_loop()
{
	for( int i =0; i < case_num_; ++i)
		cases_[i].start_case( i == 0);

	int count =0;
	while(1)  
	{
		if( cancel_) break;

		::Sleep( 1);

		for( int i =0; i < case_num_; ++i)
		{
			if( cancel_) break;
			ULONGLONG nnow =TestCaseWorkFlow::GetMillisecondTime();
			cases_[i].run_it( nnow);
		}
	}

	for( int i =0; i < case_num_; ++i)
		cases_[i].stop_case( i == 0);
}
