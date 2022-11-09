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

#include <cmsLib/wincrash/crashhandler.h>

#if EW_PLATFORM_WINDOWS
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
#include <Dbghelp.h>
#pragma comment (lib, "Dbghelp.lib")
#include <time.h>

LONG WINAPI HandleCrash(PEXCEPTION_POINTERS pExceptPtrs)
{
	if(pExceptPtrs == NULL)
	{
		// Raise an exception
		__try
		{
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, 0);
		}
		__except(HandleCrash(GetExceptionInformation()), EXCEPTION_CONTINUE_EXECUTION)
		{

		}
	}

	// Create the date/time string
	time_t curtime = time(NULL);
	tm pTime;
	localtime_s(&pTime, &curtime);
	char filename[MAX_PATH+1];
	char modname[MAX_PATH*2];
	ZeroMemory(modname, sizeof(modname));
	HANDLE hDump = INVALID_HANDLE_VALUE;
	if(GetModuleFileName(NULL, modname, MAX_PATH*2) == 0)
		strcpy_s(modname,MAX_PATH*2, ("UNKNOWN"));

	char* mname = strrchr(modname, ('\\'));
	mname++;

	// Create the directory first
	if(!PathFileExists(("CrashDumps")))
		CreateDirectory(("CrashDumps"), NULL);

	// Writing Simple Dump
	sprintf_s(filename, ("CrashDumps\\%s-%02d%02d%02d_%02d%02d%02d_%u_s.dmp"),
		mname, pTime.tm_year+1900, pTime.tm_mon+1, pTime.tm_mday,
		pTime.tm_hour, pTime.tm_min, pTime.tm_sec, GetCurrentThreadId());

	hDump = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);

	if(hDump != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION info;
		info.ClientPointers = FALSE;
		info.ExceptionPointers = pExceptPtrs;
		info.ThreadId = GetCurrentThreadId();

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDump, MiniDumpWithIndirectlyReferencedMemory, &info, 0, 0);
		CloseHandle(hDump);
	}

	// Writing Full Dump
	sprintf_s(filename, ("CrashDumps\\%s-%02d%02d%02d_%02d%02d%02d_%u_f.dmp"),
		mname, pTime.tm_year+1900, pTime.tm_mon+1, pTime.tm_mday,
		pTime.tm_hour, pTime.tm_min, pTime.tm_sec, GetCurrentThreadId());

	hDump = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0);

	if(hDump != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION info;
		info.ClientPointers = FALSE;
		info.ExceptionPointers = pExceptPtrs;
		info.ThreadId = GetCurrentThreadId();

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDump, MiniDumpWithFullMemory, &info, 0, 0);
		CloseHandle(hDump);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

#endif