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
		S_INT_64 tnow = OSSystem::mOS->GetTimestamp();
		logDebug(out_runtime, "redis user online check, last:%lld now:%lld", ltm, tnow);
		return (ltm + USER_LOSTCONN_TIME) > tnow;
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

void StateService::redis_save_userinfo(RedisClient* rdv, const SProtocolHead& head, const char* acc
	, bool disable, bool saverole, PRO::DBUserRoles& roles)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	S_INT_64 uid = head.get_token_useriid();
	S_INT_64 token = head.get_token_token();
	//acount
	std::string key = rdv->build_rediskey(rdkey::user::USER_ACCOUNT, acc);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_ACCOUNT_F_USERID, std::to_string(uid).c_str()),
		std::make_pair(rdkey::user::USER_ACCOUNT_F_TOKEN, std::to_string(token).c_str()),
	});
	rdv->pexpire(key.c_str(), REDIS_USER_LIFETIME);

	//user
	GLoc3D defpos = GLoc3D::zero_point();
	key = rdv->build_rediskey(rdkey::user::USER_USERINFO, uid);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_UINFO_F_USERID, std::to_string(uid).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_TOKEN, std::to_string(token).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_LOGINTIME, std::to_string(OSSystem::mOS->GetTimestamp()).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GIDUID, std::to_string(head.token_giduid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_SLOTTOKEN, std::to_string(head.token_slottoken_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_ROLEID, std::to_string(head.role_iid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GAMEID, std::to_string(head.gameid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GAMELOC, defpos.to_string().c_str()),
	});

	//清楚relogin标记
	rdv->del_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_RELOGIN);

	//account会直接加载rolelist
	if (saverole)
		rdv->set_hashobject(key.c_str(), rdkey::user::USER_UINFO_F_ROLES, &roles, svrApp.get_redisprotocache());

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

void StateService::redis_update_onlinestate(RedisClient* rdv, const SProtocolHead& head, const GLoc3D& pos)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	S_INT_64 userid = head.get_token_useriid();
	//更新用户gate信息
	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_UINFO_F_GIDUID, std::to_string(head.token_giduid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_SLOTTOKEN, std::to_string(head.token_slottoken_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_ROLEID, std::to_string(head.role_iid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GAMEID, std::to_string(head.gameid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GAMELOC, pos.to_string().c_str()),
		});

	S_INT_64 tnow = OSSystem::mOS->GetTimestamp();
	key = rdv->build_rediskey(rdkey::user::USER_ONLINES, get_onlinequeue_hash( userid));
	rdv->add_zset(key.c_str(), std::to_string(userid).c_str(), tnow, UpdateType::EXIST);

	logDebug(out_runtime, "user:%lld update state, gameid:%lld loc:%s", userid, head.get_gameid(), pos.to_string().c_str());
}

bool StateService::redis_user_logout(RedisClient* rdv, S_INT_64 userid, S_INT_64 token, bool checktoken)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);
	
	S_INT_64 rdtoken = 0;
	//数据不存在
	if (!rdv->get_hashmember_ul(key.c_str(), rdkey::user::USER_UINFO_F_TOKEN, rdtoken))
		return false;

	if (checktoken)
	{
		if (token != rdtoken)
			return false;
	}

	//清除relogin标记
	rdv->del_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_RELOGIN);

	//清除login状态
	key = rdv->build_rediskey(rdkey::user::USER_ONLINES, get_onlinequeue_hash(userid));
	rdv->del_zsetmember(key.c_str(), std::to_string(userid).c_str());

	logDebug(out_runtime, "redis user:[%lld] do logout", userid);

	return true;
}

void StateService::redis_gatelost_ntf(RedisClient* rdv, S_INT_64 userid, S_INT_64 token)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);
	S_INT_64 rdtoken = 0;
	//数据不存在
	if (!rdv->get_hashmember_ul(key.c_str(), rdkey::user::USER_UINFO_F_TOKEN, rdtoken))
		return;

	if (token != rdtoken)
		return;

	rdv->set_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_RELOGIN, "yes");

	logDebug(out_runtime, "redis user:[%lld] gatelost set flag", userid);
}

bool StateService::redis_user_relogin_check(RedisClient* rdv, SProtocolHead& head, S_INT_64 userid, 
	S_INT_64& ntoken, S_INT_64& roleid, S_INT_64& gameid, GLoc3D& pos)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	S_INT_64 token = ntoken;

	//必须在线
	if (!check_user_in_onlinequeue(rdv, userid))
		return false;

	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);

	boost::unordered_map<std::string, std::string> datas;
	//数据不存在
	if (!rdv->get_hashallmember(key.c_str(), datas))
		return false;

	boost::unordered_map<std::string, std::string>::iterator fiter = datas.find(rdkey::user::USER_UINFO_F_TOKEN);
	if (fiter == datas.end())
		return false;
	S_INT_64 rdtoken = ShareUtil::atoi64(fiter->second.c_str());

	if (rdtoken != token)
		return false;

	//检测relogin标记
	fiter = datas.find(rdkey::user::USER_UINFO_F_RELOGIN);
	if (fiter == datas.end())
		return false;

	//清除标记
	//清除relogin标记
	rdv->del_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_RELOGIN);

	//获取一个新的token
	ntoken = ShareUtil::get_token();

	//更新缓冲
	rdv->set_hashmember(key.c_str(), {
		std::make_pair(rdkey::user::USER_UINFO_F_TOKEN, std::to_string(ntoken).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_GIDUID, std::to_string(head.token_giduid_).c_str()),
		std::make_pair(rdkey::user::USER_UINFO_F_SLOTTOKEN, std::to_string(head.token_slottoken_).c_str()),
		});

	fiter = datas.find(rdkey::user::USER_UINFO_F_ROLEID);
	roleid = ShareUtil::atoi64(fiter->second.c_str());

	fiter = datas.find(rdkey::user::USER_UINFO_F_GAMEID);
	if (fiter == datas.end())
		gameid = 0;
	else
		gameid = ShareUtil::atoi64(fiter->second.c_str());

	fiter = datas.find(rdkey::user::USER_UINFO_F_GAMELOC);
	if (fiter == datas.end())
		pos = GLoc3D::zero_point();
	else
		GLoc3D::build_from_str(fiter->second, pos);

	logDebug(out_runtime, "relogin user:[%lld] success, roleid:%lld gameid:%lld loc:%s", userid, roleid, gameid, pos.to_string().c_str());

	return true;
}