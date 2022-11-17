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

#ifndef __VIRTUALUSER_H__
#define __VIRTUALUSER_H__

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <cmsLib/lua/ScriptContext.h>

#include <gameLib/protobuf/Proto_all.h>
#include "robotclient/PhpResult.h"

#include <map>
#include <string>
#include <list>

class TestCaseWorkFlow;

USE_PROTOCOL_NAMESPACE

class CaseConditionResult
{
public:
	CaseConditionResult():succ(false){}

	void reset(){ succ =false; }
public:
	bool succ;
};

class VirtualUser
{
public:
	static void InitScriptBind( lua_State* l);

	VirtualUser();
	virtual ~VirtualUser();

	void set_testcase(TestCaseWorkFlow* p);

	void set_userinfo(int id, const char* urladdr, const char* prefix);
	void set_openprefix(const char* prefix);
	//创建循环账号,max:最大的循环数
	void circle_openid(int max);

	void init_before_start();
	void reset_call_result() { action_result_ = 0; }
	void set_callresult(S_INT_32 v) { action_result_ = v; }

	bool run_script( const char* cmd);

public:
	//配置信息
	std::string					user_openid_;
	std::string					php_url_;
	bool						gts_connected_;

	//base info from config
	std::string					openid_prefix_;
	std::string					url_addr_;
	int							init_iid_;
	int							circle_openid_;

	//php返回信息
	std::auto_ptr<PhpResult>	svrinfo_;

	//连接的服务器信息
	std::string		gts_ip_;
	S_INT_32		gts_port_;

protected:
	ScriptContext		context_;
	CaseConditionResult	cond_result_;

	TestCaseWorkFlow	*test_;

	//保存当前的调用返回
	S_INT_32			action_result_;

public:
	void php_login_result(PhpResult* ret);
	void gts_connect_result(bool succ) { gts_connected_ = succ; }
	void random_svrinfo();

	void user_login(const char* accid);
	bool do_heartbeat(S_INT_64 tnow);

public:
	bool is_call_succ() { return action_result_ == 0; }

	//script support function or property
	bool is_phplogin() { return svrinfo_.get() != 0; }
	int  get_serversize() { return (int)svrinfo_->svrinfos.size(); }
	bool is_connected() { return gts_connected_; }

	//script support function or property
	int  logintime();
	bool islogin();
	bool has_chr();
	bool is_ready();

public:
	ULONGLONG		login_time_;
	S_INT_64		pre_heartbeat_time_;

	//账号id
	std::string		account_id_;
	S_INT_32		userid_;
	S_UINT_8		faction_;
	S_INT_32		chrid_;
	std::string		chrname_;

public:
	std::string		user_name_;
	std::string		user_pwd_;

public:
	void process_protocol(S_UINT_16 proid, BasicProtocol *p);
};

#endif	//__VIRTUALUSER_H__
