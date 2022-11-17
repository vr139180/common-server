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

#ifndef __BEFORESERVICEACTION_H__
#define __BEFORESERVICEACTION_H__

#include "testcase/core/CaseActionBase.h"

class ServerConnectAction : public CaseActionBase
{
public:
	static std::string get_done_name() { return "serverconnect"; }

public:
	ServerConnectAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual std::string get_action_name() { return get_done_name(); }
	virtual void init_before_start();
	virtual void time_out();

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do(ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

protected:
	//超时间隔
	S_INT_32	time_out_step_;
};

class LoginAction : public CaseActionBase
{
	typedef enum enum_LoginAction_State
	{
		LoginActionState_Idle =0,
		LoginActionState_lgs_conn,
		LoginActionState_lgs_loginreq,
		LoginActionState_gts_conn,
		LoginActionState_gts_conn_wait,
		LoginActionState_gts_loginreq,
	}LoginAction_State;

public:
	static std::string get_done_name(){ return "userlogin";}

public:
	LoginAction();

	virtual S_UINT_32 get_req_protoid();
	virtual std::string get_action_name() { return get_done_name(); }
	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void time_out();

protected:
	virtual void build_from_xml_sub( tinyxml2::XMLElement* act);

private:
	LoginAction_State	la_state_;

	S_INT_32		proxy_index_;
	std::string		token_;
	int				userid_;

	//超时间隔
	S_INT_32	time_out_step_;
};

class LogoutAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "userlogout";}

public:
	LogoutAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual std::string get_action_name() { return get_done_name(); }
	virtual bool log_analysis() { return false; }
	virtual void init_before_start();

protected:
	virtual void start_action();
	virtual void end_action();

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

class SelChrAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "selchr";}

public:
	SelChrAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual std::string get_action_name() { return get_done_name(); }
	virtual bool log_analysis() { return false; }
	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

class CreateChrAction : public CaseActionBase
{
public:
	static std::string get_done_name(){ return "createchr";}

public:
	CreateChrAction();

	virtual S_UINT_32 get_req_protoid() { return 0; }
	virtual std::string get_action_name() { return get_done_name(); }
	virtual bool log_analysis() { return false; }
	virtual void init_before_start();

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now);

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

};

#endif	//__BEFORESERVICEACTION_H__
