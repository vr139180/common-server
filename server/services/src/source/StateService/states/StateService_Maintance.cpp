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

static char LUA_ONLINE_MAINTANCE[] = "\
local onkey = KEYS[1]\n\
local offkey = KEYS[2]\n\
local tnow = tonumber(ARGV[1])\n\
local offusers = redis.call('ZRANGEBYSCORE', onkey, '-inf', ARGV[2])\n\
local cnt = table.maxn(offusers)\n\
if cnt > 0 then\n\
redis.call('ZREMRANGEBYSCORE', onkey, '-inf', ARGV[2])\n\
for i =1,cnt do\n\
redis.call('ZADD',offkey,tnow,offusers[i])\n\
end\n\
end\n\
return offusers\n";

static char LUA_OFFLINE_MAINTANCE[] = "\
local onkey = KEYS[1]\n\
local offkey = KEYS[2]\n\
local backtm = tonumber(ARGV[1])\n\
local offusers = redis.call('ZRANGEBYSCORE', offkey, '-inf', ARGV[2])\n\
local cnt = table.maxn(offusers)\n\
if cnt > 0 then\n\
redis.call('ZREMRANGEBYSCORE', offkey, '-inf', ARGV[2])\n\
for i =1,cnt do\n\
redis.call('ZADD',onkey,backtm,offusers[i])\n\
end\n\
end\n\
return offusers\n";

void StateService::on_onlineuser_maintance(void*)
{
	RedisClient* rdv = svrApp.get_redisclient();

	for (int ii = 0; ii < STATE_ONLINEQUEUE_HASHNUM; ++ii)
	{
		std::string onlinekey = rdv->build_rediskey(rdkey::user::USER_ONLINES, ii);
		std::string offlinekey = rdv->build_rediskey(rdkey::user::USER_OFFLINES, ii);
		std::initializer_list<StringView> keys = {
			onlinekey.c_str(), offlinekey.c_str()
		};

		std::string tnow = std::to_string(OSSystem::mOS->GetTimestamp());
		std::string checktime = std::to_string(OSSystem::mOS->GetTimestamp() - USER_LOSTCONN_TIME);
		std::initializer_list<StringView> vals = {
			tnow.c_str(), checktime.c_str()
		};

		std::vector<std::string> rets;
		if (rdv->evalStrs(LUA_ONLINE_MAINTANCE, keys, vals, rets))
		{
			if (rets.size() > 0)
			{
				logDebug(out_runtime, "stateservice run online maintance lua success, player number:%d", (int)rets.size());
			}

			for (int jj = 0; jj < rets.size(); ++jj)
			{
				S_INT_64 userid = ShareUtil::atoi64(rets[jj].c_str());
				if (userid <= 0)
					continue;

				svrApp.dispath_userlogout_process(userid);
			}
		}
		else
		{
			logError(out_runtime, "stateservice run online maintance lua failed");
		}
	}
}

void StateService::on_user_logout_process(S_INT_64 userid)
{
	RedisClient* rdv = svrApp.get_redisclient();

	//获取user信息
	std::string key = rdv->build_rediskey(rdkey::user::USER_USERINFO, userid);
	boost::unordered_map<std::string, std::string> datas;
	if (rdv->get_hashallmember(key.c_str(), datas))
	{
		PRO::User_Logout_ntf *ntf = new PRO::User_Logout_ntf();
		S_INT_64 roleid = ShareUtil::atoi64(datas[rdkey::user::USER_UINFO_F_ROLEID].c_str());
		S_INT_64 gameid = 0;
		if( datas.find(rdkey::user::USER_UINFO_F_GAMEID) != datas.end())
			gameid = ShareUtil::atoi64(datas[rdkey::user::USER_UINFO_F_GAMEID].c_str());
		ntf->set_user_iid(userid);
		ntf->set_role_iid(roleid);
		ntf->set_gameid(gameid);
		
		NetProtocol* pro = new NetProtocol(ntf);
		SProtocolHead& head = pro->write_head();
		head.token_giduid_ = ShareUtil::atoi64(datas[rdkey::user::USER_UINFO_F_GIDUID].c_str());
		head.token_slottoken_ = ShareUtil::atoi64(datas[rdkey::user::USER_UINFO_F_SLOTTOKEN].c_str());
		head.set_role_iid(roleid);
		head.set_gameid(gameid);
		
		svrApp.send_to_gate(pro);

		//同时抄送game, home
		if (roleid > 0)
			svrApp.send_to_home(pro->clone());
		if (gameid > 0)
			svrApp.send_to_game(pro->clone());

		//清除relogin标记
		rdv->del_hashmember(key.c_str(), rdkey::user::USER_UINFO_F_RELOGIN);

		logDebug(out_runtime, "online user maintance -> user:%lld be forced to logout, notify=> gate = y, home = %lld, game = %lld", userid, roleid, gameid);
	}

	std::string offlinekey = rdv->build_rediskey(rdkey::user::USER_OFFLINES, get_onlinequeue_hash(userid));
	//从offline zset中确认删除
	rdv->del_zsetmember(offlinekey.c_str(), std::to_string(userid).c_str());
}

void StateService::on_offlineuser_maintance(void*)
{
	RedisClient* rdv = svrApp.get_redisclient();

	for (int ii = 0; ii < STATE_ONLINEQUEUE_HASHNUM; ++ii)
	{
		std::string onlinekey = rdv->build_rediskey(rdkey::user::USER_ONLINES, ii);
		std::string offlinekey = rdv->build_rediskey(rdkey::user::USER_OFFLINES, ii);
		std::initializer_list<StringView> keys = {
			onlinekey.c_str(), offlinekey.c_str()
		};

		std::string backtm = std::to_string(OSSystem::mOS->GetTimestamp() - USER_LOSTCONN_TIME - 10 * 1000);
		std::string checktime = std::to_string(OSSystem::mOS->GetTimestamp() - USER_OFFDEAL_TIME);
		std::initializer_list<StringView> vals = {
			backtm.c_str(), checktime.c_str()
		};

		std::vector<std::string> rets;
		if (rdv->evalStrs(LUA_OFFLINE_MAINTANCE, keys, vals, rets))
		{
			if (rets.size() > 0)
			{
				logDebug(out_runtime, "stateservice run offline maintance lua success, rollback player number:%d", (int)rets.size());
			}
		}
		else
		{
			logError(out_runtime, "stateservice run offline maintance lua failed");
		}
	}
}
