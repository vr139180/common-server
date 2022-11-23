// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <cmsLib/GlobalSettings.h>

#ifdef EW_PLATFORM_LINUX 

#include <cmsLib/util/FileLogOnlyOnce.h>

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "OSDriverLinux.h"

extern void changemode( s32 );
extern int kbhit( void );

int OSDriverLinux::GetUnixTimestamp()
{
	std::time_t t = std::time(0);
	return (int)t;
}

u64 OSDriverLinux::timeGetTimeMy()
{
	struct timeval tv;
	gettimeofday( &tv, NULL);
	u64 ret =tv.tv_sec * 1000;
	return ret + tv.tv_usec/1000;
}

u64 OSDriverLinux::GetTimestamp()
{
	//return timeGetTimeMy();
	return mNowTickTime;
}

void OSDriverLinux::UpdateNowTick()
{
	mNowTickTime = timeGetTimeMy();
}

u64 OSDriverLinux::GetTicks()
{
    return mNowTickTime;
}

u64 OSDriverLinux::GetTicksRealtime()
{
	return timeGetTimeMy();
}

ulong OSDriverLinux::GetCalendarTime() const
{
    return (ulong)time(0);
}

void OSDriverLinux::Sleep( u32 ms )
{
    usleep( ms * 1000);
}

void OSDriverLinux::thread_yield()
{
	usleep( 1000);
	/*
	struct timeval ot;
	ot.tv_sec =0;
	ot.tv_usec =1000;
	select( 0, 0, 0, 0, &ot);
	*/
}

bool OSDriverLinux::KbHit( s32& ch )
{
    bool result = false;
    
    changemode( 1 );

    if ( kbhit() )
    {
        ch = getchar();
        result = true;
    }

    changemode(0);

    return result;
}

const char* OSDriverLinux::ProcessName()
{
	static char processName[512] ={0,};

	if( readlink( "/proc/self/exe", processName, 512) == -1)
		memcpy( &processName[0], "./", strlen("./"));

	return processName;
}

void OSDriverLinux::SetAppTitle( const std::string& title )
{
	//save pid to mypid file
	FileLogOnlyOnce flog("mypid");
	flog.write_line( "%ld", getpid());

}

ulong OSDriverLinux::GetProcessId()
{
	return getpid();
}

void changemode( s32 dir )
{
    static struct termios oldt, newt;

    if ( dir == 1 )
    {
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    }
    else
    {
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    }
}

int kbhit( void )
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    FD_ZERO( &rdfs );
    FD_SET( STDIN_FILENO, &rdfs );

    select( STDIN_FILENO+1, &rdfs, NULL, NULL, &tv );

    return FD_ISSET( STDIN_FILENO, &rdfs );
}

#endif // EW_PLATFORM_LINUX 