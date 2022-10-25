#ifndef __HASHUTIL_H__
#define __HASHUTIL_H__

#include <cmsLib/core_type.h>

/**
* @class	HashUtil
* @author	Created by Lu Jianfeng ( lujf1978@163.com ), 2022
*
* @brief Hash工具类
**/
class HashUtil
{
private:
	HashUtil() {}

public:
	//fnv1 32 hash算法
	static S_INT_32 fnv1_32_hash(const std::string& str);

	//Thomas Wang 整数hash
	static S_INT_32 int_hash(S_INT_32 key);

	//RS算法hash
	static S_INT_32 rs_hash(const std::string& str);
};

#endif //__HASHUTIL_H__
