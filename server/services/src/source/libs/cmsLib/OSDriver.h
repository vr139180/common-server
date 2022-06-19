#ifndef OSDRIVER_H
#define OSDRIVER_H

#include <cmsLib/GlobalSettings.h>

typedef struct DateTime_s
{
    s32			mSeconds;					// Second [0,59]
    s32			mMinutes;					// Minute [0,59]
    s32			mHours;						// Hour   [0,23]
    s32			mDay;						// Day    [1,31]
    s32			mMonths;					// Month  [0,11]
    s32			mYears;						// Year, From 0, 1900, 2011...
    s32			mWeekDays;					// day of week [0, 6]
    s32			mYearDays;					// day of year [0,365]
    s32			mIsDaySavingsTime;			// ....
} DateTime_t;


class OSDriver
{
public:
    virtual                 ~OSDriver();

    virtual void            UpdateNowTick() = 0;
    virtual u64             GetTicks() = 0;
	virtual u64             GetTicksRealtime() = 0;
	virtual u64				GetTimestamp() =0;
    virtual ulong           GetCalendarTime() const = 0;

	virtual int				GetUnixTimestamp() = 0;

    virtual void            Sleep( u32 ms ) = 0;

    virtual bool            KbHit( s32& ch ) = 0;

    virtual const char*		ProcessName() = 0;

	virtual void            SetAppTitle( const std::string& title ) = 0;

    virtual ulong           GetProcessId() = 0;

	virtual void			thread_yield() =0;
};

#endif //OSDRIVER_H
