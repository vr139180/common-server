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
