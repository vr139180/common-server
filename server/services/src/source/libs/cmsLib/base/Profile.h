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

#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/OSDriver.h>
#include <cmsLib/base/OSSystem.h>

class Profile 
{
public:
	Profile(const char* funcName, u32& num, ulong delay);

	Profile(const char* funcName, ulong delay);

	Profile(const char* funcName, ulong delay, s64 userData);

	~Profile();

private:
    u32             mEmpty;
    u32&            mRunTimes;
    u64				mStartTime;
    const char*  mFuncName;
    ulong           mDelay;
    s64             mUserData;
};


//#define PROFILE()           Profile     profile( EW_FUNC, 100 )
//#define PROFILE2(num)       Profile     profile( EW_FUNC, num, 100 )
//#define PROFILE3(time,ud)   Profile     profile( EW_FUNC, time, ud )

#define PROFILE()
#define PROFILE2(num)
#define PROFILE3(time,ud)

#endif // __PROFILE_H__