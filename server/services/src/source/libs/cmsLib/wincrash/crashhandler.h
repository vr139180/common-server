#ifndef HANDLECRASH_CPP
#define HANDLECRASH_CPP

#include <cmsLib/GlobalSettings.h>
#if EW_PLATFORM_WINDOWS

#include "windows.h"
LONG WINAPI HandleCrash(PEXCEPTION_POINTERS pExceptPtrs);

#endif

#endif//HANDLECRASH_CPP