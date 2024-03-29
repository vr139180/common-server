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
#ifndef __GLOBALSETTINGS_H__
#define __GLOBALSETTINGS_H__

#include <cmsLib/cmsLib.h>

#if defined(_WIN32) || defined(_WIN64)

	#define EW_PLATFORM_WINDOWS 1

	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
	#endif

#else
	#define EW_PLATFORM_LINUX 1

#endif

#if defined(EW_PLATFORM_WINDOWS)
	//#define _CRTDBG_MAP_ALLOC
    //#include <crtdbg.h>
    #if defined(_MSC_VER)
        #pragma warning(disable:4996)       // warning c4996: 'function': was declared deprecated.
        #pragma warning(disable:4512)       // warning c4512: 'class': assignment operator could not be generated.
        #pragma warning(disable:4100)       // warning c4100: 'identifier' : unreferenced formal parameter.
        #define _CRT_SECURE_NO_WARNINGS 1
    #endif
#elif defined(EW_PLATFORM_LINUX)
    #include <byteswap.h>
    #include <sys/types.h>
    #include <time.h>
#endif

#ifdef __cplusplus

    #include <string>
    #include <bitset>
    #include <vector>
    #include <stack>
    #include <list>
    #include <set>
    #include <queue>
    #include <deque>
    #include <map>
    #include <algorithm>
    #include <fstream>
    #include <iostream>
    #include <locale>
    #include <sstream>
    #include <functional>

    typedef std::string             StringA;
    typedef std::wstring            StringW;

    #ifdef UNICODE
        typedef std::wstring        String;
        typedef std::wofstream      Ofstream;
        typedef std::wifstream      Ifstream;
    #else
        typedef std::string         String;
        typedef std::ofstream       Ofstream;
        typedef std::ifstream       Ifstream;
    #endif

    typedef std::bitset<8>          bit8Data_t;

#endif // __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <typeinfo>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <climits>

typedef unsigned char               u8;
typedef signed char                 s8;
typedef char                        c8;
typedef unsigned short              u16;
typedef signed short                s16;
typedef wchar_t                     wchar;
typedef unsigned int                u32;
typedef signed int                  s32;
typedef float                       f32;
typedef double                      f64;
typedef unsigned long               ulong;
typedef signed long                 slong;
typedef signed long long            s64;
typedef unsigned long long          u64;
typedef u32                         ewhandle_t;
typedef size_t                      msize_t;
typedef std::vector<String>         stringSet_t;
typedef std::vector<s32>            vecInt_t;

#if defined(EW_PLATFORM_WINDOWS)

    #define     EW_FUNC         __FUNCTION__

#elif defined(EW_PLATFORM_LINUX)

    #define     EW_FUNC         __FUNCTION__
        //         __func__
#else
#endif

#if defined(EW_PLATFORM_WINDOWS)
    #define EW_INLINE           __forceinline

	#define strcasecmp			stricmp

#elif defined(EW_PLATFORM_LINUX)
    #define EW_INLINE           inline

	#define __int64				long

#elif defined(EW_PLATFORM_FREEBSD)
#endif

#endif // __GLOBALSETTINGS_H__
