#ifndef OSDRIVERWIN32_H
#define OSDRIVERWIN32_H

#include <cmsLib/GlobalSettings.h>

#ifdef EW_PLATFORM_WINDOWS

#include <cmsLib/OSDriver.h>

class OSDriverWin32 : public OSDriver
{
public:
                            OSDriverWin32();
							~OSDriverWin32();

    virtual void            UpdateNowTick();
    virtual u64             GetTicks();
	virtual u64             GetTicksRealtime();
	virtual u64				GetTimestamp();
    virtual ulong           GetCalendarTime() const;
    virtual void            Sleep( u32 ms );

	virtual int				GetUnixTimestamp();

    virtual bool            KbHit( s32& ch );

    virtual const char*		ProcessName();

	virtual void            SetAppTitle( const std::string& title );

    virtual ulong           GetProcessId();

	virtual void			thread_yield();

private:
    u64                     mNowTickTime;
};

#endif //EW_PLATFORM_WINDOWS

#endif // OSDRIVERWIN32_H
