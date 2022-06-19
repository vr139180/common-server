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