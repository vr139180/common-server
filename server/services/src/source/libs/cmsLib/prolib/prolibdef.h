/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __PROLIBDEF__H__
#define __PROLIBDEF__H__

//if prolib used by client, please comment this defined
//#define PROLIB_USED_BY_SERVERSIDE	1

#if PROLIB_USED_BY_SERVERSIDE
	#include "cmsLib/memory/CoreMemory.h"
	#define PRO_NEW_EX	PROTOCOL_NEW

	#define PROLIB_ASSERT( x)	assert( x)
#else
	#define PRO_NEW_EX	new

	#define PROLIB_ASSERT( x)	assert( x)
#endif

#endif	//__PROLIBDEF__H__
