#ifndef __SHAREUTIL__H__
#define __SHAREUTIL__H__

#include <list>
#include <vector>
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/prolib/core_type.h>
#include <time.h>

/**
* @class ShareUtil
* 
* @brief 常用工具函数封装
**/
class ShareUtil
{
private:
	ShareUtil(){}

public:
	/**
	* 分割字符串
	* @param src
	* @param splitchr
	* @param ret
	* @return
	**/
	static bool splitstr2int( const char* src, const char* splitchr, std::vector<int>& ret);
	static bool splitstr2int64(const char* src, const char* splitchr, std::vector<S_INT_64>& ret);

	/**
	* 合并int值和str值为一个string
	* @param v1
	* @param v2
	* @return 
	**/
	static std::string combineintstr( int v1, const char* v2, int radix= 10);
	static std::string combinestrint( const char* leftv, int rightv, int radix =10);

	static int atoi( const char* str, int radix= 10);
	static S_INT_64 atoi64(const char* str, int radix = 10);

	/**
	* 拆分字符串
	* @param src
	* @param splitchr
	* @param ret
	* @return
	**/
	static bool splitstr2str( const char* src, const char* splitchr, std::list<std::string>& ret);

	static void SplitString( const char* cmd, stringSet_t& strSet, const char splitor, bool IgnoreSpace = false );

	static void encrypt_mask( c8* buf, int len, S_UINT_64 mask);
	static void decrypt_mask( c8* buf, int len, S_UINT_64 mask);

	static void encrypt_tea( S_UINT_32 *v, S_UINT_32 *k, int cnum =16);
	static void decrypt_tea( S_UINT_32 *v, S_UINT_32 *k, int cnum =16);

	static time_t time_gm( tm* t);
	static struct tm * gmt_time( time_t t);
    
    //return the miss times between last refresh time to now
    static int getRefreshMissTimes( time_t lastrefreshtime, int hour, int min = 0, int sec = 0 );
    //reture the last refresh time before the check time
    static time_t getLastRefreshTime( time_t checktime, int hour, int min = 0, int sec = 0 );
    // 返回1-7表示周一到周日 
    static int getTodayWeekday();
};

#endif	//__SHAREUTIL__H__
