#ifndef __UTILSERVICEACTION_H__
#define __UTILSERVICEACTION_H__

#include "testcase/core/CaseActionBase.h"

class IdleAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "idle";}

public:
	IdleAction();

	virtual void init_before_start();

	virtual void handle_message( BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub( tinyxml2::XMLElement* act);

private:
	ULONGLONG	idle_begin_time_;

	int			idle_time_;
};

#endif	//__UTILSERVICEACTION_H__
