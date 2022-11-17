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

#ifndef __UTILSERVICEACTION_H__
#define __UTILSERVICEACTION_H__

#include "testcase/core/CaseActionBase.h"

class EndTestAction : public CaseActionBase
{
public:
	static std::string get_done_name() { return "endtest"; }

public:
	EndTestAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual bool log_analysis() { return false; }
	virtual std::string get_action_name() { return get_done_name(); }

	virtual void init_before_start();

protected:
	virtual void start_action();
	virtual void end_action();

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

private:
};

class IdleAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "idle";}

public:
	IdleAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual bool log_analysis() { return false; }
	virtual std::string get_action_name() { return get_done_name(); }

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

//根据开始的userid计算延迟开始时间
class DelayStartAction : public CaseActionBase
{
public:
	static std::string get_done_name() { return "delaystart"; }

public:
	DelayStartAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual bool log_analysis() { return false; }
	virtual std::string get_action_name() { return get_done_name(); }

	virtual void init_before_start();

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do(ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

private:
	//间隔的userid区间
	int			user_range_delay_;

	ULONGLONG	idle_begin_time_;
	int			idle_time_;
};

class CallFailAction : public CaseActionBase
{
public:
	static std::string get_done_name() { return "callfail"; }

public:
	CallFailAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual bool log_analysis() { return true; }
	virtual std::string get_action_name() { return get_done_name(); }

	virtual void init_before_start();

protected:
	virtual void start_action();
	virtual void end_action();

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

private:
};

#endif	//__UTILSERVICEACTION_H__
