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
