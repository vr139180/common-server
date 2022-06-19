#ifndef OSDRIVERLINUX_H
#define OSDRIVERLINUX_H

#include <cmsLib/OSDriver.h>
#include <cmsLib/GlobalSettings.h>

#ifdef EW_PLATFORM_LINUX 

class OSDriverLinux : public OSDriver
{
public:
    virtual ~OSDriverLinux(){}

	virtual void            UpdateNowTick();
	virtual u64				GetTimestamp();

	virtual u64             GetTicks();
	virtual u64             GetTicksRealtime();
    virtual ulong           GetCalendarTime() const;
    virtual void            Sleep( u32 ms );

	virtual int				GetUnixTimestamp();

    virtual bool            KbHit( s32& ch );

    virtual const char*		ProcessName();

    virtual void            SetAppTitle( const std::string& title );

    virtual ulong           GetProcessId();

	u64						timeGetTimeMy();

	virtual void			thread_yield();

private:
	u64                     mNowTickTime;
};

#endif // EW_PLATFORM_LINUX 

#endif // OSDRIVERLINUX_H
