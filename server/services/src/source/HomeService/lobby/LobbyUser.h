#ifndef __LOBBYUSER_H__
#define __LOBBYUSER_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/lua/ScriptContext.h>

#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "lobby/user/UserCacheData.h"
#include "lobby/user/UserRoles.h"

class LobbyService;

typedef enum tagUserState {
	//空闲
	UserState_Free = 0,
	UserState_RolesLoading,
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

	void init_user(S_INT_64 giduid, S_INT_64 slottoken);
	void rest_user();

	bool is_samesession(S_INT_64 utoken);
	bool set_usertoken(BasicProtocol* msg);

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
	//run_xmlobjective函数在LobbyUser_Lua.cpp中实现
	//目的是为了保持函数名称和实现的一致
	virtual bool run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret);

	virtual int get_role_level();
	virtual int get_bag_itemnum(S_INT_64 itemiid);

	virtual bool is_end_of_taskgroup(S_INT_32 gid);
	virtual std::string get_luaojb_name();

protected:
	bool sync_rolelist();

protected:
	//用户状态
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
	ProtoTokenUtil::parse_usertoken(utoken, slot, ptoken);

	return (this->slot_ == slot && slottoken_ == utoken);
}

#endif //__LOBBYUSER_H__
