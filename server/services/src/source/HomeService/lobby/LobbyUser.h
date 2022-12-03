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

#ifndef __LOBBYUSER_H__
#define __LOBBYUSER_H__

#include <cmsLib/core_type.h>
#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/lua/ScriptContext.h>

#include <gameLib/protobuf/cpp/db_internal.pb.h>
#include <gameLib/commons/GLoc3D.h>

#include "lobby/user/UserCacheData.h"
#include "lobby/user/UserRoles.h"

class LobbyService;

typedef enum tagUserState {
	//空闲
	UserState_Free = 0,
	UserState_RolesReady,
	//等待db加载
	UserState_RoleDetailLoading,
	//准备完毕
	UserState_Ready,
}UserState;

class LobbyUser : public UserCacheData, public IUserDataEnv
{
public:
	LobbyUser();
	virtual ~LobbyUser();

	LobbyService* get_owner() { return owner_; }
	void set_context(LobbyService* p);

	void sync_head(const SProtocolHead& head);
	void rest_user();

	virtual S_INT_64 get_user_iid() { return s_head_.get_token_useriid(); }

	bool is_samesession( const SProtocolHead& head);

	UserState get_userstate() { return cur_state_; }
	bool is_user_ready() { return cur_state_ == UserState::UserState_Ready; }
	bool is_user_free() { return cur_state_ == UserState::UserState_Free; }

	//选择角色后的通知
	void db_role_selected_done();
	//加载后台的通知
	void db_sync_load_done();

	void send_to_gate(BasicProtocol* msg);
	void send_to_game(BasicProtocol* msg, S_INT_64 gameid);

public:
	//from network
	//role list request
	void on_ls_rolelist_req();
	void on_ls_rolecreate_req(const char* nickname);
	void on_ls_roleselect_req(S_INT_64 roleid);
	void on_ls_rolelocsave_ntf(S_INT_64 roleid, const GLoc3D& loc);

public:
	//from database
	void on_db_rolelist_update( UserRoles& from);
	void notify_roledetail_to_user();

public:
	virtual void task_data_loaded();

	static void bind_luacontext(lua_State* l);
	//-------------------------implement IUserDataEnv-------------------------------
	//run_xmlobjective函数在LobbyUser_Lua.cpp中实现
	//目的是为了保持函数名称和实现的一致
	virtual bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret);

	virtual int get_role_level();
	virtual int get_bag_itemnum(S_INT_64 itemiid);

	virtual bool is_end_of_taskgroup(S_INT_32 gid);
	virtual std::string get_luaojb_name();

protected:
	//从redis同步角色列表
	bool redis_sync_rolelist();

protected:
	SProtocolHead	s_head_;
	//用户状态
	UserState	cur_state_;

	//roles cache
	UserRoles	roles_data_;

protected:
	LobbyService*	owner_;
};

inline
bool LobbyUser::is_samesession(const SProtocolHead& head)
{
	return s_head_.is_same_session(head);
}

#endif //__LOBBYUSER_H__
