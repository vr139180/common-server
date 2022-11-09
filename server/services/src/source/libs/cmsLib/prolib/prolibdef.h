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
