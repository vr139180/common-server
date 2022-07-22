#pragma once

#include "stdafx.h"
#include <cmsLib/prolib/ProtocolFactory.h>
#include <gameLib/protobuf/Proto_all.h>
#include <cmsLib/lua/ScriptContext.h>

#include <string>

USE_PROTOCOL_NAMESPACE

#define MAX_PACK_LEN	16*1024
#define SINGLE_PACK_LEN 1024*16

class CommandTestImpl
{
public:
	CommandTestImpl( );
	virtual ~CommandTestImpl(void);

    void set_wnd( HWND h){ parent_wnd_ =h;};

public:
	int RunLuaShellCmd( std::string& cmd);
    bool IsScriptInit();

	//lgs socket
	bool send_to_lgs( BasicProtocol *p);
	BasicProtocol* recv_from_lgs(S_UINT_16& proiid);
	bool connect_to_lgs();
	void disconnect_to_lgs();

	//gts socket
	bool send_to_gts( BasicProtocol* p);
	BasicProtocol* recv_from_gts(S_UINT_16& proiid);
	bool connect_to_gts();
	void disconnect_to_gts();

	std::string get_status();

	bool islogon();
	bool isplayersel();

	void InitScriptBind( lua_State* l);

public:
	//状态信息
	CString		ret_desc_;

	S_INT_64	uid;
	std::string ut;

	std::string lgs_ip_;
	int			lgs_port_;
	S_INT_64	login_token_;
	int			login_slot_;

	std::string gts_ip_;
	int			gts_port_;

public:
	//1:lgs 2:gts 3:lgs+gts
	S_INT_8		thread_step_;

private:
    void startThread();
    void stopThread();
    
    static DWORD  WINAPI Thread(LPVOID  lparam);

	void lgs_linkdo();
	void gts_linkdo();

private:
	ScriptContext context_;

	ProtocolFactory *data_;
    HANDLE	hthd;
    DWORD	dwThdId;
    //bool    lock_;
    HWND    parent_wnd_;

	//lgs socket
	SOCKET	socket_;
	char recv_buffer_[MAX_PACK_LEN];
	int	 data_len_;

	//gts
	SOCKET	socket2_;
	char gts_recv_buffer_[MAX_PACK_LEN];
	int	 gts_data_len_;

public:
	NS_STL::string	username_;
	S_INT_64		userid_;
	S_INT_64		role_iid_;

public:
	//实现的脚本功能
	void ping();
	void on_ping_ntf(BasicProtocol* pro, CString* pRetMsg);

	void login(const char* name, const char* pwd);
	void logintoken();
	void on_login_ack(BasicProtocol* pro, CString* pRetMsg);

	void logout();

	void on_rolelist_ack(BasicProtocol* pro, CString* pRetMsg);

	void roleselect(S_INT_64 rid);
	void on_roleselect_ack(BasicProtocol* pro, CString* pRetMsg);

	void rolecreate(const char* nickname);
	void on_rolecreate_ack(BasicProtocol* pro, CString* pRetMsg);

	void on_roledetaila_ack(BasicProtocol* pro, CString* pRetMsg);
	void on_roledetailb_ack(BasicProtocol* pro, CString* pRetMsg);
	void on_roledetailend_ack(BasicProtocol* pro, CString* pRetMsg);

	void build_additem(S_INT_64 parent, S_INT_64 resid);
	void on_build_additem_ack(BasicProtocol* pro, CString* pRetMsg);
	void build_delitem(S_INT_64 buildiid);
	void on_build_delitem_ack(BasicProtocol* pro, CString* pRetMsg);

	void pet_adoptone(S_INT_64 petid);
	void on_pet_adoptone_ack(BasicProtocol* pro, CString* pRetMsg);
	void pet_releaseone(S_INT_64 mypetiid);
	void on_pet_releaseone_ack(BasicProtocol* pro, CString* pRetMsg);

	//--------------------------chat friend task------------------------
	void chat_say(int t, S_INT_64 cid, const char* content);
	void on_chat_chatmsg_ntf(BasicProtocol* pro, CString* pRetMsg);

	//--------------------------mail------------------------
	void mail_user_active();
	void on_mail_newmail_ntf(BasicProtocol* pro, CString* pRetMsg);

	void mail_new_systemmail( const char* title, const char* content);
	void on_mail_new_systemmail_ack(BasicProtocol* pro, CString* pRetMsg);

	void mail_new_usermail(const char* title, const char* content);
	void on_mail_new_usermail_ack(BasicProtocol* pro, CString* pRetMsg);

	void mail_getmail(S_INT_64 mailiid, bool next);
	void on_mail_getmail_ack(BasicProtocol* pro, CString* pRetMsg);

	void mail_read(S_INT_64 mailiid);
	void on_mail_read_ack(BasicProtocol* pro, CString* pRetMsg);

	void mail_delete(S_INT_64 mailiid);
	void on_mail_delete_ack(BasicProtocol* pro, CString* pRetMsg);

	//--------------------------friend------------------------
	void friend_invite(S_INT_64 toiid);
	void on_friend_invite_ack(BasicProtocol* pro, CString* pRetMsg);

	void friend_inviteconfirm(S_INT_64 iid, bool agree);
	void on_friend_inviteconfirm_ack(BasicProtocol* pro, CString* pRetMsg);

	void friend_delete(S_INT_64 iid);
	void on_friend_delete_ack(BasicProtocol* pro, CString* pRetMsg);

	void friend_get(S_INT_64 invite, S_INT_32 find);
	void on_friend_get_ack(BasicProtocol* pro, CString* pRetMsg);

	//--------------------------task--------------------------
	void task_waitlist();
	void on_task_waitlist_ack(BasicProtocol* pro, CString* pRetMsg);

	void task_mytasks();
	void on_task_mytasks_ack(BasicProtocol* pro, CString* pRetMsg);

	void task_get(S_INT_32 taskiid);
	void on_task_get_ack(BasicProtocol* pro, CString* pRetMsg);

	void task_submit(S_INT_32 taskiid);
	void on_task_submit_ack(BasicProtocol* pro, CString* pRetMsg);

	void on_task_obtainreward_ntf(BasicProtocol* pro, CString* pRetMsg);

	void task_giveup(S_INT_32 taskiid);
	void on_task_giveup_ack(BasicProtocol* pro, CString* pRetMsg);
};
