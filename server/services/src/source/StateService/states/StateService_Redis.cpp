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
#include <gameLib/global_const.h>

#include "StateServiceApp.h"

USE_PROTOCOL_NAMESPACE

S_INT_32 StateService::get_onlinequeue_hash(S_INT_64 uiid)
{
	return uiid % STATE_ONLINEQUEUE_HASHNUM;
}

bool StateService::check_user_in_onlinequeue(RedisClient* rdv, S_INT_64 userid)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(rdkey::user::USER_ONLINES, get_onlinequeue_hash(userid));
	std::string struid = rdv->build_rediskey("%lld", userid);
	S_INT_64 ltm = 0;
	rdv->get_zscore(key.c_str(), struid.c_str(), ltm);
	if (ltm > 0)
	{
		//最后一次更新时间是否超过30s
		return (ltm + USER_LOSTCONN_TIME) > OSSystem::mOS->GetTimestamp();
	}
	else
	{
		return false;
	}
}

bool StateService::check_user_disable(RedisClient* rdv, const char* acc, S_INT_64& useriid)
{
	useriid = 0;
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(rdkey::user::USER_ACCOUNT, acc);

	rdv->get_hashmember_ul(key.c_str(), rdkey::user::USER_ACCOUNT_F_USERID, useriid);
	if (useriid == 0)
		return false;

	key = rdv->build_rediskey(rdkey::user::USER_USERINFO, useriid);
	//检测账户是否被禁用
	if (rdv->has_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_DISABLE))
		return true;

	return false;
}

void StateService::redis_save_userinfo(RedisClient* rdv, const SProtocolHead& head, const char* acc, S_INT_64 uid, S_INT_64 token, bool disable)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	//acount
	std::string key = rdv->build_rediskey(rdkey::user::USER_ACCOUNT, acc);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_ACCOUNT_F_USERID, std::to_string(uid).c_str()),
		std::make_pair(rdkey::user::USER_ACCOUNT_F_TOKEN, std::to_string(token).c_str()),
	});
	rdv->pexpire(key.c_str(), REDIS_USER_LIFETIME);

	//user
	key = rdv->build_rediskey(rdkey::user::USER_USERINFO, uid);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_UINFO_F_USERID, std::to_string(uid).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_TOKEN, std::to_string(token).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_LOGINTIME, std::to_string(token).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GIDUID, std::to_string(head.token_giduid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_SLOTTOKEN, std::to_string(head.token_slottoken_).c_str()),
	});
	if (disable)
		rdv->set_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_DISABLE, "1");

	rdv->pexpire( key.c_str(), REDIS_USER_LIFETIME);

	//online
	if (!disable)
	{
		key = rdv->build_rediskey(rdkey::user::USER_ONLINES, get_onlinequeue_hash(uid));
		rdv->add_zset(key.c_str(), std::to_string(uid).c_str(), OSSystem::mOS->GetTimestamp());
	}
}

void StateService::redis_update_onlinestate(RedisClient* rdv, S_INT_64 userid, const SProtocolHead& head)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	//更新用户gate信息
	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_UINFO_F_GIDUID, std::to_string(head.token_giduid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_SLOTTOKEN, std::to_string(head.token_slottoken_).c_str()),
		});

	S_INT_64 tnow = OSSystem::mOS->GetTimestamp();
	key = rdv->build_rediskey(rdkey::user::USER_ONLINES, get_onlinequeue_hash(userid));
	rdv->add_zset(key.c_str(), std::to_string(userid).c_str(), tnow, UpdateType::EXIST);
}