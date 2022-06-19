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