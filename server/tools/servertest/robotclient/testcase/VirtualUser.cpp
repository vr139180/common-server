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

#include "StdAfx.h"

#include "VirtualUser.h"

#include "testcase/core/TestCaseWorkFlow.h"
#include <cmsLib/util/ShareUtil.h>

VirtualUser::VirtualUser():
	userid_( NO_INITVALUE),
	chrid_( NO_INITVALUE)
{
	test_ = 0;
	context_.init_scriptcontext();

	context_.regist_2_context<VirtualUser>( "vuser", this);
	context_.regist_2_context<CaseConditionResult>( "result", &cond_result_);
}

VirtualUser::~VirtualUser()
{
	context_.uninit_scriptcontext();
}

void VirtualUser::set_testcase(TestCaseWorkFlow* p)
{
	this->test_ = p;
}

void VirtualUser::set_userinfo(int id, const char* urladdr, const char* prefix)
{
	init_iid_ = id;
	circle_openid_ = 0;
	openid_prefix_ = prefix;
	url_addr_ = urladdr;

	user_openid_ = ShareUtil::str_format("%s_%d", prefix, init_iid_);
	php_url_ = ShareUtil::str_format("%s&openid=%s", urladdr, user_openid_.c_str());
}

void VirtualUser::set_openprefix(const char* prefix)
{
	openid_prefix_ = prefix;

	if (circle_openid_ == 0)
	{
		user_openid_ = ShareUtil::str_format("%s_%d", openid_prefix_.c_str(), init_iid_);
		php_url_ = ShareUtil::str_format("%s&openid=%s", url_addr_.c_str(), user_openid_.c_str());
	}
	else
	{
		user_openid_ = ShareUtil::str_format("%s_%d-%d", openid_prefix_.c_str(), init_iid_, circle_openid_);
		php_url_ = ShareUtil::str_format("%s&openid=%s", url_addr_.c_str(), user_openid_.c_str());
	}
}

void VirtualUser::circle_openid(int max)
{
	++circle_openid_;
	if (circle_openid_ >= max)
		circle_openid_ = 0;

	if (circle_openid_ == 0)
	{
		user_openid_ = ShareUtil::str_format("%s_%d", openid_prefix_.c_str(), init_iid_);
		php_url_ = ShareUtil::str_format("%s&openid=%s", url_addr_.c_str(), user_openid_.c_str());
	}
	else
	{
		user_openid_ = ShareUtil::str_format("%s_%d-%d", openid_prefix_.c_str(), init_iid_, circle_openid_);
		php_url_ = ShareUtil::str_format("%s&openid=%s", url_addr_.c_str(), user_openid_.c_str());
	}
}

void VirtualUser::random_svrinfo()
{
	svrinfo_->get_ipinfo(gts_ip_, gts_port_);
}

void VirtualUser::init_before_start()
{
	pre_heartbeat_time_ = NO_INITVALUE;
	login_time_ = NO_INITVALUE;

	account_id_ = "";
	userid_ =NO_INITVALUE;
	chrid_ =NO_INITVALUE;
}

int VirtualUser::logintime()
{
	if( login_time_ == NO_INITVALUE)
		return 0;

	return (int)(TestCaseWorkFlow::GetMillisecondTime() - login_time_);
}

bool VirtualUser::islogin()
{
	return userid_ != NO_INITVALUE;
}

void VirtualUser::user_login(const char* accid)
{
	this->account_id_ = accid;

	this->login_time_ = TestCaseWorkFlow::GetMillisecondTime();
	this->pre_heartbeat_time_ = this->login_time_;
}

void VirtualUser::php_login_result(PhpResult* ret)
{
	svrinfo_.reset(ret);
	if (ret != 0)
	{
		account_id_ = ShareUtil::str_format<64>("%lld",ret->account_id_).c_str();
	}
}

bool VirtualUser::has_chr()
{
	return true;
}

bool VirtualUser::is_ready()
{
	return chrid_ != NO_INITVALUE;
}

bool VirtualUser::do_heartbeat(S_INT_64 tnow)
{
	if (pre_heartbeat_time_ == NO_INITVALUE)
		return false;

	if ((pre_heartbeat_time_ + 60 * 1000) > tnow)
		return false;

	pre_heartbeat_time_ = tnow;

	return true;
}

#define VU_PROCESS_MESSAGE( msgid, fun )    \
	case msgid: \
	fun( p); \
	break;

void VirtualUser::process_protocol(S_UINT_16 proid, BasicProtocol *p)
{

}