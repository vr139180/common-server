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

#ifndef __LOBBYSERVICE_H__
#define __LOBBYSERVICE_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>
#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/redis/RedisProtoBufThreadCache.h>
#include <cmsLib/lua/ScriptContext.h>

#include <taskLib/service/ITaskEnv.h>

#include "lobby/LobbyUser.h"
#include "lobby/LobbyUserContainer.h"

class LobbyService : public VirtualMainThread , public MessageProcess, public IGlobalDataEnv
{
	typedef VirtualMainThread base;
public:
	LobbyService();
	virtual ~LobbyService();

	void init_lobby();

	LobbyUser* get_usercheck_from_msg(NetProtocol* msg);
	LobbyUser* get_userbyid_from_msg(NetProtocol* msg);
	LobbyUser* get_userbyid_from_msg(const SProtocolHead& head);

	//vnode发生变化后，对用户数据进行维护
	void vnode_cluster_maintance();

protected:
	void init_luacontext();
	void uninit_luacontext();

protected:
	void on_vnode_cluster_maintance( void*);

	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}
	virtual void NetProcessMessage(NetProtocol* message, bool& autorelease);

public:
	//---------------------------IGlobalDataEnv interface -------------------------------
	static void bind_luacontext(lua_State* l);

	virtual ScriptContext* get_lua_context();
	virtual S_INT_64 new_taskgroup_iid();
	virtual S_INT_64 new_task_iid();

	virtual void task_luacontext_begin(IGlobalDataEnv* genv, IUserDataEnv* uenv);
	virtual void task_luacontext_end();

protected:
	void on_lb_userlogout_ntf(NetProtocol* pro, bool& autorelease);
	void on_lb_rolelist_req(NetProtocol* pro, bool& autorelease);
	void on_lb_rolecreate_req(NetProtocol* pro, bool& autorelease);
	void on_lb_roleselect_req(NetProtocol* pro, bool& autorelease);
	void on_lb_saveroleloc_ntf(NetProtocol* pro, bool& autorelease);

	void on_lb_mysimpleinfo_req(NetProtocol* pro, bool& autorelease);

	void on_lb_build_additem_req(NetProtocol* pro, bool& autorelease);
	void on_lb_build_delitem_req(NetProtocol* pro, bool& autorelease);

	void on_lb_pet_adoptone_req(NetProtocol* pro, bool& autorelease);
	void on_lb_pet_releaseone_req(NetProtocol* pro, bool& autorelease);

	void on_lb_task_waitlist_req(NetProtocol* pro, bool& autorelease);
	void on_lb_task_mytasks_req(NetProtocol* pro, bool& autorelease);
	void on_lb_task_get_req(NetProtocol* pro, bool& autorelease);
	void on_lb_task_submit_req(NetProtocol* pro, bool& autorelease);
	void on_lb_task_giveup_req(NetProtocol* pro, bool& autorelease);

private:
	//users
	LobbyUserContainer<LobbyUser>	lobby_users_;

	RedisClient					redis_;
	RedisProtoBufThreadCache	redisproto_cache_;

	ScriptContext				lua_context_;
};

#endif //__LOBBYSERVICE_H__
