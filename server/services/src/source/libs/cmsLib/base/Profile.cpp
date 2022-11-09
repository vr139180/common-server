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

#include <cmsLib/base/Profile.h>

Profile::Profile(const char* funcName, u32& num, ulong delay)
	:mRunTimes(num),
	mFuncName(funcName),
	mDelay(delay),
	mUserData(0)
{
	mStartTime = OSSystem::mOS->GetTicks();
}

Profile::Profile(const char* funcName, ulong delay)
	:mRunTimes(mEmpty),
	mFuncName(funcName),
	mDelay(delay),
	mUserData(0)
{
	mEmpty = 1;
	mStartTime = OSSystem::mOS->GetTicks();
}

Profile::Profile(const char* funcName, ulong delay, s64 userData)
	:mRunTimes(mEmpty),
	mFuncName(funcName),
	mDelay(delay),
	mUserData(userData)
{
	mEmpty = 1;
	mStartTime = OSSystem::mOS->GetTicks();
}

Profile::~Profile()
{
	u64 d = OSSystem::mOS->GetTicks() - mStartTime;
	if (d >= mDelay)
	{
		printf("%s \n\t\tcost %u ms, run %u times, user data %d\n", mFuncName, d, mRunTimes, (s32)mUserData);
	}
}