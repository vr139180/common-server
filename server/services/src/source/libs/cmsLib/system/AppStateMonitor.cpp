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

#include <cmsLib/system/AppStateMonitor.h>

#include <boost/bind/bind.hpp>

AppStateMonitor::AppStateMonitor():
	is_actived_( false)
{
}

void AppStateMonitor::regist_to_monitor( AppStateMonitorItem *pItem)
{
	if( pItem == 0)
		return;

	monitor_items_.push_back( pItem);
}

void AppStateMonitor::init_monitor( const char* logfile, TimerContainer* tc, int step)
{
	log_file_ =logfile;

	tc->add_timer( step, boost::BOOST_BIND( &AppStateMonitor::on_autosave, this, 
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void AppStateMonitor::monitor_allstate( FileLogOnlyOnce& flog)
{
	std::list<AppStateMonitorItem*>::iterator iter =monitor_items_.begin(), eiter =monitor_items_.end();
	for( ; iter != eiter; ++iter)
	{
		AppStateMonitorItem* pItem =(*iter);
		//save action at every monitor item
		flog.write_noline( "%d,", pItem->get_action());
		pItem->save( flog);
	}
}

void AppStateMonitor::save_to_log()
{
	if( !is_actived_)
		return;

	FileLogOnlyOnce flog( log_file_.c_str());

	//save basic information
	flog.write_line( "%d", time(0));

	monitor_allstate( flog);

	is_actived_ =false;
}

void AppStateMonitor::on_autosave( u64 tnow, int interval, u64 iid, bool& finish)
{
	//if timer is work, 
	app_active();

	save_to_log();
}