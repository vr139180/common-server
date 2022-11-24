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

#include "states/StateService.h"

#include <cmsLib/util/ShareUtil.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/redis/user_redis_const.h>

#include "StateServiceApp.h"
#include "dbs/DBSCtrl.h"
#include "dbs/cmd/UserLoginCmd.h"

USE_PROTOCOL_NAMESPACE

void StateService::InitNetMessage()
{
	REGISTERMSG(USER_PROTYPE::USER_LOGIN_REQ, &StateService::on_user_login_req, this);
	REGISTERMSG(USER_PROTYPE::USER_RELOGIN_REQ, &StateService::on_user_relogin_req, this);
	REGISTERMSG(USER_PROTYPE::USER_ACTIVE_NTF, &StateService::on_user_active_ntf, this);
	REGISTERMSG(USER_PROTYPE::USER_GATELOST_NTF, &StateService::on_user_gatelost_ntf, this);
	REGISTERMSG(USER_PROTYPE::USER_LOGOUT_NTF, &StateService::on_user_logout_ntf, this);
}

void StateService::on_user_login_req(NetProtocol* pro, bool& autorelease)
{
	User_Login_req* req = dynamic_cast<User_Login_req*>(pro->msg_);

	logDebug(out_runtime, "recv user:%s login req...", req->account().c_str());
	std::string acc = req->account().c_str();
	ShareUtil::to_lower(acc);

	RedisClient* rdv = svrApp.get_redisclient();
	S_INT_64 useriid = 0;
	if (check_user_disable(rdv, acc.c_str(), useriid))
	{
		User_Login_ack *ack = new User_Login_ack();
		ack->set_type(req->type());
		ack->set_result(1);

		svrApp.send_to_gate(pro->head_, ack);
		return;
	}
	else if (useriid > 0 && check_user_in_onlinequeue( rdv, useriid))
	{
		//检测是否已经在线
		User_Login_ack *ack = new User_Login_ack();
		ack->set_type(req->type());
		ack->set_result(6);

		svrApp.send_to_gate(pro->head_, ack);
		return;
	}

	UserLoginCmd *pcmd = new UserLoginCmd(req->account().c_str(), req->pwd().c_str(), (useriid <= 0), this);
	pcmd->reuse_cmd(pro->head_);

	DBSCtrl::instance()->post_head_db_cmd( pcmd);
}

void StateService::on_db_user_login_act( SProtocolHead& head, S_INT_32 result, S_INT_32 type
	, const char* account, bool saverole, PRO::DBUserRoles& roles)
{
	S_INT_64 uid = head.get_token_useriid();
	//验证成功检测 是否在线
	if (result == 0 && check_user_in_onlinequeue(0, uid))
		result = 6;

	User_Login_ack *ack = new User_Login_ack();
	ack->set_type(type);
	ack->set_result(result);

	//验证成功
	if (result == 0 || result == 1)
	{
		//登陆使用的token为gate传递过来的
		S_INT_64 token = head.get_token_token();

		ack->set_user_iid(uid);
		ack->set_logintoken( token);

		//save to redis
		redis_save_userinfo(0, head, account, (result == 1), saverole, roles);
	}

	svrApp.send_to_gate( head, ack);
}

void StateService::on_user_relogin_req(NetProtocol* pro, bool& autorelease)
{
	User_ReLogin_req* req = dynamic_cast<User_ReLogin_req*>(pro->msg_);

	logDebug(out_runtime, "recv user:[%lld,%lld] relogin req...", req->user_iid(), req->logintoken());

	RedisClient* rdv = svrApp.get_redisclient();
	S_INT_64 roleid = 0, gameid = 0, userid = 0, newtoken = 0;
	userid = req->user_iid();
	newtoken = req->logintoken();
	bool succ = redis_user_relogin_check( rdv, pro->head_, userid, newtoken, roleid, gameid);
	
	User_ReLogin_ack* ack = new User_ReLogin_ack();
	ack->set_result(1);
	if (succ)
	{
		ack->set_result(0);
		ack->set_user_iid(userid);
		ack->set_logintoken(newtoken);
		ack->set_role_iid(roleid);
		ack->set_gameid(gameid);
	}

	SProtocolHead head = pro->head_;
	svrApp.send_to_gate(head, ack);
}

void StateService::on_user_active_ntf(NetProtocol* pro, bool& autorelease)
{
	User_Active_ntf *ntf = dynamic_cast<User_Active_ntf*>(pro->msg_);
	redis_update_onlinestate( 0, pro->head_, ntf->gameid());
}

void StateService::on_user_gatelost_ntf(NetProtocol* pro, bool& autorelease)
{
	redis_gatelost_ntf(0, pro->get_useriid(), pro->head_.get_token_token());
}

void StateService::on_user_logout_ntf(NetProtocol* pro, bool& autorelease)
{
	if (redis_user_logout( 0, pro->get_useriid(), pro->head_.get_token_token()))
	{
		User_Logout_ntf* ntf = dynamic_cast<User_Logout_ntf*>(pro->msg_);

		//logout成功，返回datarouter. 同时抄送game
		autorelease = false;
		svrApp.send_to_datarouter(pro);
	}
}
