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

class LobbyService : public VirtualMainThread , public MessageProcess, public IGlobalDataEnv
{
	typedef VirtualMainThread base;
public:
	LobbyService();
	virtual ~LobbyService();

	void init_lobby(int pindex, int nums, std::vector<LobbyUser*>& lus);
	void reset_syscmd();

	LobbyUser* get_user_byslot(int slot);
	LobbyUser* get_user_byslot(int slot, S_INT_64 token);
	LobbyUser* get_userbyslot_from_msg(BasicProtocol* msg);
	LobbyUser* get_userofsame_from_msg(BasicProtocol* msg);
	LobbyUser* get_userofsame_from_x(S_INT_64 uid, S_INT_64 token);

protected:
	void init_luacontext();
	void uninit_luacontext();

protected:
	void reset_lobby( void*);
	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(BasicProtocol* message, bool& autorelease, int msgid) {}

public:
	//---------------------------IGlobalDataEnv interface -------------------------------
	static void bind_luacontext(lua_State* l);

	virtual ScriptContext* get_lua_context();
	virtual S_INT_64 new_taskgroup_iid();
	virtual S_INT_64 new_task_iid();

	virtual void task_luacontext_begin(IGlobalDataEnv* genv, IUserDataEnv* uenv);
	virtual void task_luacontext_end();

protected:
	void on_lb_ghuserinit_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_ghuserlogout_ntf(BasicProtocol* pro, bool& autorelease);

	void on_lb_ghroledetail_ask(BasicProtocol* pro, bool& autorelease);

	void on_lb_rolecreate_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_roleselect_req(BasicProtocol* pro, bool& autorelease);

	void on_lb_build_additem_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_build_delitem_req(BasicProtocol* pro, bool& autorelease);

	void on_lb_pet_adoptone_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_pet_releaseone_req(BasicProtocol* pro, bool& autorelease);

	void on_lb_task_waitlist_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_task_mytasks_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_task_get_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_task_submit_req(BasicProtocol* pro, bool& autorelease);
	void on_lb_task_giveup_req(BasicProtocol* pro, bool& autorelease);

private:
	//users
	std::vector<LobbyUser*>		users_;
	int piece_index_;
	int piece_start_;
	int piece_end_;

	RedisClient					redis_;
	RedisProtoBufThreadCache	redisproto_cache_;

	ScriptContext				lua_context_;
};

#endif //__LOBBYSERVICE_H__
