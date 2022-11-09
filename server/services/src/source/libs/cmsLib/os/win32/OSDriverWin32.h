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
