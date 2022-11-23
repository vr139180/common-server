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
    virtual ulong           GetCalendarTime() const;
    virtual void            Sleep( u32 ms );

	virtual int				GetUnixTimestamp();

    virtual bool            KbHit( s32& ch );

    virtual const char*		ProcessName();

    virtual void            SetAppTitle( const std::string& title );

    virtual ulong           GetProcessId();

	u64						timeGetTimeMy();

	virtual void			thread_yield();

protected:
	virtual u64             GetTicksRealtime();

private:
	volatile u64			mNowTickTime;
};

#endif // EW_PLATFORM_LINUX 

#endif // OSDRIVERLINUX_H
