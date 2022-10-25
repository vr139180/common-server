/**
* 重定义了协议使用的数据类型
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2021
*/
#ifndef __CORE_TYPE__H__
#define __CORE_TYPE__H__

#include <cmsLib/GlobalSettings.h>

#define PROTOCOL_NAMESPACE_BEGIN	namespace PRO {
#define PROTOCOL_NAMESPACE_END		}
#define USE_PROTOCOL_NAMESPACE		using namespace PRO;

#ifdef _USE_32BIT_TIME_T
	#undef _USE_32BIT_TIME_T
#endif	//_USE_32BIT_TIME_T

#define S_INT_64	__int64
#define S_UINT_64	unsigned __int64

#define	S_INT_32	int
#define S_UINT_32	unsigned int

#define S_INT_16	short
#define S_UINT_16	unsigned short

#define S_INT_8		char
#define S_UINT_8	unsigned char

#define S_FLOAT_32	float

#ifdef _USE_32BIT_TIME_T
	#define S_TIMESTAMP		long
#else
	#define S_TIMESTAMP		unsigned __int64
#endif	//unsigned int

#define S_ULONG_32		unsigned long
#define S_ULONG_64		unsigned __int64

#define S_BITFIELD(PARAMNAME)	unsigned char ##PARAMNAME:1

#define NS_STL	std

#define	NO_INITVALUE	-1

#define PARAM_UNUSED(pv)	((void)&pv)

#endif //__CORE_TYPE__H__
