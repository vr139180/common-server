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

#include "OSDriverWin32.h"

#ifdef EW_PLATFORM_WINDOWS

#include <Windows.h>
#include <MMSystem.h>
#include <conio.h>
#include <psapi.h>
#include <cmsLib/Log.h>
#include <ctime>
#include <chrono>

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "psapi.lib" )

OSDriverWin32::OSDriverWin32()
{
}

OSDriverWin32::~OSDriverWin32()
{
    HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

    if ( handle != INVALID_HANDLE_VALUE )
    {
        WORD color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        color = color & 0x0f;
        SetConsoleTextAttribute( handle, color );
    }
}

int OSDriverWin32::GetUnixTimestamp()
{
	std::time_t t = std::time(0);
	return (int)t;
}

void OSDriverWin32::UpdateNowTick()
{
    mNowTickTime = GetTicksRealtime();
}

u64 OSDriverWin32::GetTicks()
{
    return mNowTickTime;
}

u64 OSDriverWin32::GetTicksRealtime()
{
	/*
	SYSTEMTIME st;
	GetSystemTime(&st);

	FILETIME time;
	SystemTimeToFileTime(&st, &time);

	LONGLONG ret = 0;

	ret = time.dwHighDateTime;
	ret = ret << 32;
	ret |= time.dwLowDateTime;
	ret /= 10;

	return ret/1000; //to millsecond
	*/
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	return ms.count();
}

u64 OSDriverWin32::GetTimestamp()
{
	//return GetTicksRealtime();
	return mNowTickTime;
}

ulong OSDriverWin32::GetCalendarTime() const
{
    return (ulong)(::time(NULL));
}

void OSDriverWin32::Sleep( u32 ms )
{
    ::Sleep( ms );
}

void OSDriverWin32::thread_yield()
{
	::Sleep( 1);
}

bool OSDriverWin32::KbHit( s32& ch )
{
    if ( _kbhit() )
    {
        ch = _getch();

        return true;
    }

    return false;
}

const char*  OSDriverWin32::ProcessName()
{
    static char processName[MAX_PATH];

    if ( GetModuleFileName( NULL, processName, MAX_PATH ) == 0 )
    {
        logError(out_runtime, "Can not get current process name, error no %u\n", GetLastError() );
        processName[0] = '\0';
    }

    return processName;
}

void OSDriverWin32::SetAppTitle( const std::string& title )
{
    SetConsoleTitle( title.c_str() );
}

ulong OSDriverWin32::GetProcessId()
{
    return GetCurrentProcessId();
}

#endif //EW_PLATFORM_WINDOWS