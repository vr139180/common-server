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

#ifndef __STATESERVICE_H__
#define __STATESERVICE_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>
#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/redis/RedisProtoBufThreadCache.h>
#include <cmsLib/lua/ScriptContext.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>
#include <gameLib/commons/GLoc3D.h>

class StateService : public VirtualMainThread , public MessageProcess
{
	typedef VirtualMainThread base;
public:
	StateService();
	virtual ~StateService();

	void init_state();
	void reset_syscmd();

private:
	RedisClient					redis_;
	RedisProtoBufThreadCache	redisproto_cache_;

protected:
	void reset_state( void*);
	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}

protected:
	//根据userid获取在线队列hash
	S_INT_32 get_onlinequeue_hash(S_INT_64 uiid);
	bool check_user_in_onlinequeue( RedisClient* rdv, S_INT_64 userid);
	//useriid > 0表示，用户存在于redis
	bool check_user_disable(RedisClient* rdv, const char* acc, S_INT_64& useriid);
	//保存账户关联到redis
	void redis_save_userinfo(RedisClient* rdv, const SProtocolHead& head, const char* acc
		, bool disable, bool saverole, PRO::DBUserRoles& roles);
	//更新激活时间
	void redis_update_onlinestate(RedisClient* rdv, const SProtocolHead& head, const GLoc3D& pos);

	//退出登录
	bool redis_user_logout(RedisClient* rdv, S_INT_64 userid, S_INT_64 token, bool checktoken = true);
	void redis_gatelost_ntf(RedisClient* rdv, S_INT_64 userid, S_INT_64 token);

	bool redis_user_relogin_check(RedisClient* rdv, SProtocolHead& head, S_INT_64 userid, 
		S_INT_64& ntoken, S_INT_64& roleid, S_INT_64& gameid, GLoc3D& pos);

public:
	//------------------------------------maintance-------------------------------------
	void on_onlineuser_maintance(void*);
	void on_offlineuser_maintance(void*);
	void on_user_logout_process(S_INT_64 userid);

	//-------------------------------------net process----------------------------------
	void on_user_login_req(NetProtocol* pro, bool& autorelease);
	//result 0: 成功 1:账号被禁用 2:账号不存在 3:验证错误 4:系统错误 5:登陆排队中
	void on_db_user_login_act( SProtocolHead& head, S_INT_32 result, S_INT_32 type
		, const char* account, bool saverole, PRO::DBUserRoles& roles);

	void on_user_relogin_req(NetProtocol* pro, bool& autorelease);
	void on_user_active_ntf(NetProtocol* pro, bool& autorelease);
	void on_user_gatelost_ntf(NetProtocol* pro, bool& autorelease);
	void on_user_logout_ntf(NetProtocol* pro, bool& autorelease);
};

#endif //__STATESERVICE_H__
