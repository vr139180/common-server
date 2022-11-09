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

#ifndef __APPSTATEMONITOR_H__
#define __APPSTATEMONITOR_H__

#include <cmsLib/util/FileLogOnlyOnce.h>
#include <cmsLib/system/TimerContainer.h>

#include <string>
#include <list>

class AppStateMonitorItem
{
public:
	virtual int get_action() =0;

	virtual void save( FileLogOnlyOnce& flog) =0;
};

class AppStateMonitor
{
public:
	AppStateMonitor();

	void init_monitor( const char* logfile, TimerContainer* tc, int step);

	void app_active(){ is_actived_ =true;}

	void on_autosave( u64 tnow, int interval, u64 iid, bool& finish);

protected:
	void monitor_allstate( FileLogOnlyOnce& flog);

	void save_to_log();

	void regist_to_monitor( AppStateMonitorItem *pItem);

private:
	bool			is_actived_;
	std::string		log_file_;

	std::list<AppStateMonitorItem*>	monitor_items_;
};

#endif	//__APPSTATEMONITOR_H__
