#ifndef __APPMONITORLOG_H__
#define __APPMONITORLOG_H__

#include <gameLib/logstash/LogRecordBase.h>
#include <gameLib/protobuf/cpp/eureka_internal.pb.h>

class AppMonitorLog : public LogRecordBase
{
public:
	AppMonitorLog();

protected:
	virtual bool to_json(std::string& jstr);
	virtual std::string get_log_cat() { return out_monitor; }

protected:
	S_INT_64				service_iid_;
	PRO::ERK_SERVICETYPE	type_;
};

#endif //__APPMONITORLOG_H__
