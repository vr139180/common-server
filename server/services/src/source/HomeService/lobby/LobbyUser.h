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

#include "lobby/user/UserCacheData.h"
#include "lobby/user/UserRoles.h"

class LobbyService;

typedef enum tagUserState {
	//����
	UserState_Free = 0,
	UserState_RolesLoading,
	UserState_RolesReady,
	//�ȴ�db����
	UserState_RoleDetailLoading,
	//׼�����
	UserState_Ready,
}UserState;

class LobbyUser : public UserCacheData, public IUserDataEnv
{
public:
	LobbyUser();
	virtual ~LobbyUser();

	LobbyService* get_owner() { return owner_; }
	void set_context(LobbyService* p);

	void init_user(S_INT_64 giduid, S_INT_64 slottoken);
	void rest_user();

	bool is_samesession(S_INT_64 utoken);

	void set_userslot(int slot) { slot_ = slot; }
	int get_userslot() { return slot_; }

	UserState get_userstate() { return cur_state_; }
	bool is_user_ready() { return cur_state_ == UserState::UserState_Ready; }

	void role_selected_done();

public:
	//from network
	//role list request
	void on_ls_rolelist_req();
	void on_ls_rolecreate_req(const char* nickname);
	void on_ls_roleselect_req(S_INT_64 roleid);

public:
	//from database
	//true:init false update
	void on_db_rolelist_update(bool initorupdate, UserRoles& from);
	void notify_roledetail_to_user();

public:
	virtual void task_data_loaded();

	static void bind_luacontext(lua_State* l);
	//-------------------------implement IUserDataEnv-------------------------------
	//run_xmlobjective������LobbyUser_Lua.cpp��ʵ��
	//Ŀ����Ϊ�˱��ֺ������ƺ�ʵ�ֵ�һ��
	virtual bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret);

	virtual int get_role_level();
	virtual int get_bag_itemnum(S_INT_64 itemiid);

	virtual bool is_end_of_taskgroup(S_INT_32 gid);
	virtual std::string get_luaojb_name();

protected:
	bool sync_rolelist();

protected:
	//�û�״̬
	UserState	cur_state_;

	//roles cache
	UserRoles	roles_data_;

protected:
	//slot
	int				slot_;
	LobbyService*	owner_;
};

inline
bool LobbyUser::is_samesession(S_INT_64 utoken)
{
	int slot = -1;
	S_INT_64 ptoken = 0;

	return (this->slot_ == slot && slottoken_ == utoken);
}

#endif //__LOBBYUSER_H__
