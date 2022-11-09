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

#include "cmsLib/util/ShareUtil.h"

#include <algorithm>
#include <stdlib.h>
#include <cmsLib/base/OSSystem.h>

bool ShareUtil::splitstr2str( const char* src, const char* splitchr, std::list<std::string>& ret)
{
	if( src == 0 || strlen( src) == 0)
		return false;
	if( splitchr == 0 || strlen( splitchr) == 0)
		return false;

	char* token =0;
	char* s =(char*)src;

	token =strtok( s, splitchr);
	while( token != NULL)
	{
		ret.push_back( token);

		token =strtok( NULL, splitchr);
	}

	return true;
}

bool ShareUtil::splitstr2int( const char* src, const char* splitchr, std::vector<int>& ret)
{
	if( src == 0 || strlen( src) == 0)
		return false;
	if( splitchr == 0 || strlen( splitchr) == 0)
		return false;

	char* token =0;
	char* s =(char*)src;

	token =strtok( s, splitchr);
	while( token != NULL)
	{
		int v =atoi( token);
		ret.push_back( v);

		token =strtok( NULL, splitchr);
	}

	return true;
}

bool ShareUtil::splitstr2int64(const char* src, const char* splitchr, std::vector<S_INT_64>& ret)
{
	if (src == 0 || strlen(src) == 0)
		return false;
	if (splitchr == 0 || strlen(splitchr) == 0)
		return false;

	char* token = 0;
	char* s = (char*)src;

	token = strtok(s, splitchr);
	while (token != NULL)
	{
		S_INT_64 v = atoi64(token);
		ret.push_back(v);

		token = strtok(NULL, splitchr);
	}

	return true;
}

std::string ShareUtil::combineintstr( int v1, const char* v2, int radix)
{
	std::string ret ="";
	if( radix == 10)
	{
		char buf[16] ={0,};
		sprintf( buf, "%d", v1);
		ret =buf;
		ret += v2;
	}
	else
		assert(false);

	return ret;
}

std::string ShareUtil::combinestrint( const char* leftv, int rightv, int radix)
{
	std::string ret( leftv);

	if( radix == 10)
	{
		char buf[16] ={0,};
		sprintf( buf, "%d", rightv);
		ret += buf;
	}
	else
		assert(false);

	return ret;
}

int ShareUtil::atoi( const char* str, int radix)
{
	char *endptr =0;
	return (int)strtol( str, &endptr, radix);
}

S_INT_64 ShareUtil::atoi64(const char* str, int radix)
{
	char *endptr = 0;
	return strtoll(str, &endptr, radix);
}

void ShareUtil::SplitString( const char* cmd, stringSet_t& strSet, const char splitor, bool IgnoreSpace)
{
	const char* p = cmd;
	const char* begin = p;

	std::string subStr;
	bool inAString = false;

	while ( *p )
	{
		if ( *p == '\"' )
		{
			inAString = !inAString;
			p++;
			continue;
		}

		if ( *p != splitor || inAString )
		{
			p++;
		}
		else
		{
			//if ( p != begin )
			{
				subStr.assign( begin, p - begin );
				strSet.push_back( subStr );
				p++;
			}
			begin = p;
		}
	}

	if ( p != begin && !*p)
	{
		subStr.assign( begin, p - begin );
		strSet.push_back( subStr );
	}
}

void ShareUtil::encrypt_mask( c8* buf, int len, S_UINT_64 mask)
{
#ifndef QUICK_LAUNCH
	int l1 =len / 8;
	S_UINT_64* pd =(S_UINT_64*)buf;
	for( int ii =0; ii < l1; ++ii)
	{
		(*pd) ^= mask;
		++pd;
	}
#endif
}

void ShareUtil::decrypt_mask( c8* buf, int len, S_UINT_64 mask)
{
#ifndef QUICK_LAUNCH
	int l1 =len / 8;
	S_UINT_64* pd =(S_UINT_64*)buf;
	for( int ii =0; ii < l1; ++ii)
	{
		(*pd) ^= mask;
		++pd;
	}
#endif
}

time_t ShareUtil::time_gm( tm* t)
{
#ifdef GMT_SAVEAS_LOCALETIME
	return mktime( t);
#else

	#ifdef EW_PLATFORM_WINDOWS
		return _mkgmtime( t);
	#else
		return timegm( t);
	#endif
#endif
}

struct tm * ShareUtil::gmt_time( time_t t)
{
#ifdef GMT_SAVEAS_LOCALETIME
	return localtime( &t);
#else
	return gmtime( &t);
#endif
}

#define TEA_DELTA_DEF	0x9e3779b9

void ShareUtil::encrypt_tea( S_UINT_32 *v, S_UINT_32 *k, int cnum)
{
	/* a key schedule constant */
	S_UINT_32 delta= TEA_DELTA_DEF;

	S_UINT_32 v0=v[0], v1=v[1], sum=0;
	S_UINT_32 k0=k[0], k1=k[1], k2=k[2], k3=k[3];

	/* basic cycle start */
	for( int i=0; i < cnum; i++)
	{
		sum += delta;
		v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
	}
	/* end cycle */

	v[0]=v0; v[1]=v1;
}

void ShareUtil::decrypt_tea( S_UINT_32 *v, S_UINT_32 *k, int cnum)
{
	S_UINT_32 delta = TEA_DELTA_DEF;

	S_UINT_32 v0=v[0], v1=v[1], sum=0;
	S_UINT_32 k0=k[0], k1=k[1], k2=k[2], k3=k[3];

	if( cnum == 16)
		sum = delta << 4;
	else if( cnum == 32)
		sum = delta << 5;
	else
		sum = delta << cnum;

	for( int i=0; i < cnum; i++)
	{
		v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
		v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		sum -= delta;                                   
	}
	
	v[0]=v0; v[1]=v1;
}

int ShareUtil::getRefreshMissTimes( time_t lastrefreshtime, int hour, int min /*= 0*/, int sec /*= 0 */ )
{
    assert( hour >= 0 && hour < 24 );
    assert( min >= 0 && min < 60 );
    assert( sec >= 0 && sec < 60 );
    if ( lastrefreshtime < 0 )
    {
        lastrefreshtime = 0;
    }
    tm nextRefreshTime = *gmtime( &lastrefreshtime );
    //this means the last fresh time will be tomorrow
    if ( ( nextRefreshTime.tm_hour > hour ) ||
        ( nextRefreshTime.tm_hour == hour && nextRefreshTime.tm_min > min ) ||
        ( nextRefreshTime.tm_hour == hour && nextRefreshTime.tm_min == min && nextRefreshTime.tm_sec >= sec ) )
    {
        time_t tomorrow = lastrefreshtime + 24 * 60 * 60;
        nextRefreshTime = *gmtime( &tomorrow );
    }
    nextRefreshTime.tm_hour = hour;
    nextRefreshTime.tm_min = min;
    nextRefreshTime.tm_sec = sec;
    time_t nexttime = ShareUtil::time_gm( &nextRefreshTime );
    time_t curtime = OSSystem::mOS->GetCalendarTime();
    if ( curtime < nexttime )
    {
        return 0;
    }
    return ( curtime - nexttime ) / ( 24 * 60 * 60 ) + 1;
}

int ShareUtil::getTodayWeekday()
{
    time_t now = OSSystem::mOS->GetCalendarTime();
    const struct tm *curtime = gmtime(&now);

    if (curtime->tm_wday == 0)
    {
        return 7;
    }
    else
    {
        return curtime->tm_wday;
    }
}

time_t ShareUtil::getLastRefreshTime( time_t checktime, int hour, int min /*= 0*/, int sec /*= 0 */ )
{
    assert( hour >= 0 && hour < 24 );
    assert( min >= 0 && min < 60 );
    assert( sec >= 0 && sec < 60 );
    if ( checktime < 0 )
    {
        checktime = 0;
    }
    tm lastRefreshTime = *gmtime( &checktime );
    //this means the last fresh time is yesterday
    if ( ( lastRefreshTime.tm_hour < hour ) ||
        ( lastRefreshTime.tm_hour == hour && lastRefreshTime.tm_min < min ) ||
        ( lastRefreshTime.tm_hour == hour && lastRefreshTime.tm_min == min && lastRefreshTime.tm_sec <= sec ) )
    {
        time_t yesterday = checktime - 24 * 60 * 60;
        lastRefreshTime = *gmtime( &yesterday );
    }
    lastRefreshTime.tm_hour = hour;
    lastRefreshTime.tm_min = min;
    lastRefreshTime.tm_sec = sec;
    return ShareUtil::time_gm( &lastRefreshTime );
}

void ShareUtil::to_lower(std::string& str)
{
	std::transform(str.cbegin(), str.cend(), str.begin(), ::tolower);
}

void ShareUtil::to_upper(std::string& str)
{
	std::transform(str.cbegin(), str.cend(), str.begin(), ::toupper);
}
