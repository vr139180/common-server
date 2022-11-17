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

#include "StdAfx.h"

#include "CommandTestImpl.h"
#include "LuaBridge/LuaBridge.h"
#include "ScriptDlg.h"

#define CMDMESSAGE( msgid, fun )    \
	case msgid: \
	fun( recv, pstr); \
	break;

void CommandTestImpl::gts_linkdo()
{
	if( socket2_ == INVALID_SOCKET)
		return;

	S_UINT_16 proiid = 0;
	BasicProtocol *recv =recv_from_gts(proiid);
	if( recv == 0)
		return;

	std::auto_ptr<BasicProtocol> p(recv);

	CString *pstr =new CString();

	CString fm;
	fm.Format("recv message��%d\r\n", proiid);
	*pstr += fm;

	switch( proiid)
	{
	//CMDMESSAGE(CLIENT_PING_NTF,         on_ping_ntf);
	CMDMESSAGE(USER_PROTYPE::USER_LOGIN_ACK, on_login_ack);
	CMDMESSAGE(USER_PROTYPE::USER_ROLELIST_ACK, on_rolelist_ack);
	CMDMESSAGE(USER_PROTYPE::USER_ROLECREATE_ACK, on_rolecreate_ack);

	CMDMESSAGE(USER_PROTYPE::USER_ROLESELECT_ACK, on_roleselect_ack);
	CMDMESSAGE(USER_PROTYPE::USER_ROLEDETAILA_NTF, on_roledetaila_ack);
	CMDMESSAGE(USER_PROTYPE::USER_ROLEDETAILB_NTF, on_roledetailb_ack);
	CMDMESSAGE(USER_PROTYPE::USER_ROLEDETAILEND_NTF, on_roledetailend_ack);

	//building
	CMDMESSAGE(BUILD_PROTYPE::BUILD_ADDITEM_ACK, on_build_additem_ack);
	CMDMESSAGE(BUILD_PROTYPE::BUILD_DELITEM_ACK, on_build_delitem_ack);

	//pets
	CMDMESSAGE(PET_PROTYPE::PET_ADOPTONE_ACK, on_pet_adoptone_ack);
	CMDMESSAGE(PET_PROTYPE::PET_RELEASEONE_ACK, on_pet_releaseone_ack);

	//chat
	CMDMESSAGE(CHAT_PROTYPE::CHAT_CHATMSG_NTF, on_chat_chatmsg_ntf);

	//mail
	CMDMESSAGE(MAIL_PROTYPE::MAIL_NEWMAIL_NTF, on_mail_newmail_ntf);
	CMDMESSAGE(MAIL_PROTYPE::MAIL_SYSTEMMAIL_ACK, on_mail_new_systemmail_ack);
	CMDMESSAGE(MAIL_PROTYPE::MAIL_NEWMAIL_ACK, on_mail_new_usermail_ack);
	CMDMESSAGE(MAIL_PROTYPE::MAIL_MAILGET_ACK, on_mail_getmail_ack);
	CMDMESSAGE(MAIL_PROTYPE::MAIL_READMAIL_ACK, on_mail_read_ack);
	CMDMESSAGE(MAIL_PROTYPE::MAIL_DELETEMAIL_ACK, on_mail_delete_ack);

	//friend
	CMDMESSAGE(FRIEND_PROTYPE::FRD_FRIENDINVITE_ACK, on_friend_invite_ack);
	CMDMESSAGE(FRIEND_PROTYPE::FRD_INVITECONFIRM_ACK, on_friend_inviteconfirm_ack);
	CMDMESSAGE(FRIEND_PROTYPE::FRD_FRIENDDELETE_ACK, on_friend_delete_ack);
	CMDMESSAGE(FRIEND_PROTYPE::FRD_FRIENDLIST_ACK, on_friend_get_ack);

	//task
	CMDMESSAGE(TASK_PROTYPE::TASK_WAITLIST_ACK, on_task_waitlist_ack);
	CMDMESSAGE(TASK_PROTYPE::TASK_MYTASKLIST_ACK, on_task_mytasks_ack);
	CMDMESSAGE(TASK_PROTYPE::TASK_GETTASK_ACK, on_task_get_ack);
	CMDMESSAGE(TASK_PROTYPE::TASK_SUBMITTASK_ACK, on_task_submit_ack);
	CMDMESSAGE(TASK_PROTYPE::TASK_OBTAINREWARD_NTF, on_task_obtainreward_ntf);
	CMDMESSAGE(TASK_PROTYPE::TASK_GIVEUPTASK_ACK, on_task_giveup_ack);

	default:
		if (proiid != 0)
		{
			fm.Format("δ������Ϣ��%d\r\n", proiid);
			*pstr += fm;
		}
		break;
	}

	if ( pstr->GetLength() > 0)
		PostMessage( parent_wnd_, WM_USER+0x200, (WPARAM)pstr, 0);
	else
		delete pstr;
}

void ScriptDlg::regist()
{
	//֧�ֵĽű�����
	add_function("serverinfo()", "��ȡ��������Ϣ");
	add_function("ping()", "Ping ������");
	add_function("login('test1','123456')", "db��½\r\nusername,password");
	add_function("logintoken()", "token��½\r\n");
	add_function("logout()", "ע��");
	add_function("rolecreate('test')", "������ɫ");
	add_function("roleselect(1)", "ѡ���ɫ");

	//building
	add_function("build_additem(0,1)", "��һ������");
	add_function("build_delitem(1)", "ɾ��һ������");

	//pets
	add_function("pet_adoptone(1)", "����һ������");
	add_function("pet_releaseone(1)", "����һ������");

	//chat
	add_function("chat_say(2,100,'abc xxx U')", "����������Ϣ 2,4,5  100,userid,200");

	//mail
	add_function("mail_useractive()", "��������");
	add_function("mail_sysmail_send('test-system','system mail contents')", "����ϵͳ�ʼ�");
	add_function("mail_usermail_send('test-user','user mail contents')", "�����û��ʼ�");
	add_function("mail_getmail(0,true)", "��ȡ�û��ʼ�");
	add_function("mail_read(1)", "��ȡ�û��ʼ�");
	add_function("mail_delete(1)", "ɾ���û��ʼ�");

	//friend
	add_function("friend_invite(1)", "������Ӻ���");
	add_function("friend_inviteconfirm(1,true)", "ȷ����Ӻ�������");
	add_function("friend_delete(1)", "ɾ������");
	add_function("friend_get(0,0)", "��ȡ�����б�");

	//task
	add_function("task_waitlist()", "��ȡ���Խӵ������б�");
	add_function("task_mytasks()", "�ҵĵ������б�");
	add_function("task_get(1)", "��ָ��������");
	add_function("task_submit(1)", "�ύָ��������");
	add_function("task_giveup(1)", "����ָ��������");
}

void CommandTestImpl::InitScriptBind(lua_State* l)
{
	using namespace luabridge;

	getGlobalNamespace(l)
		.beginNamespace("test")
		.beginClass <CommandTestImpl>("CommandTest")
		.addConstructor <void(*) (void)>()
		.addFunction("serverinfo", &CommandTestImpl::get_serverinfo)
		.addFunction("ping", &CommandTestImpl::ping)
		.addFunction("login", (void (CommandTestImpl::*)(const char*, const char*))&CommandTestImpl::login)
		.addFunction("logintoken", (void (CommandTestImpl::*)(void))&CommandTestImpl::logintoken)
		.addFunction("logout", (void (CommandTestImpl::*)(void))&CommandTestImpl::logout)
		.addFunction("rolecreate", (void (CommandTestImpl::*)(const char*))&CommandTestImpl::rolecreate)
		.addFunction("roleselect", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::roleselect)
		.addFunction("build_additem", (void (CommandTestImpl::*)(S_INT_64,S_INT_64))&CommandTestImpl::build_additem)
		.addFunction("build_delitem", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::build_delitem)
		.addFunction("pet_adoptone", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::pet_adoptone)
		.addFunction("pet_releaseone", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::pet_releaseone)
		.addFunction("chat_say", (void (CommandTestImpl::*)(int,S_INT_64,const char*))&CommandTestImpl::chat_say)
		.addFunction("mail_useractive", (void (CommandTestImpl::*)(void))&CommandTestImpl::mail_user_active)
		.addFunction("mail_sysmail_send", (void (CommandTestImpl::*)(const char*, const char*))&CommandTestImpl::mail_new_systemmail)
		.addFunction("mail_usermail_send", (void (CommandTestImpl::*)(const char*, const char*))&CommandTestImpl::mail_new_usermail)
		.addFunction("mail_getmail", (void (CommandTestImpl::*)(S_INT_64,bool))&CommandTestImpl::mail_getmail)
		.addFunction("mail_read", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::mail_read)
		.addFunction("mail_delete", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::mail_delete)
		.addFunction("friend_invite", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::friend_invite)
		.addFunction("friend_inviteconfirm", (void (CommandTestImpl::*)(S_INT_64,bool))&CommandTestImpl::friend_inviteconfirm)
		.addFunction("friend_delete", (void (CommandTestImpl::*)(S_INT_64))&CommandTestImpl::friend_delete)
		.addFunction("friend_get", (void (CommandTestImpl::*)(S_INT_64,S_INT_32))&CommandTestImpl::friend_get)
		.addFunction("task_waitlist", (void (CommandTestImpl::*)(void))&CommandTestImpl::task_waitlist)
		.addFunction("task_mytasks", (void (CommandTestImpl::*)(void))&CommandTestImpl::task_mytasks)
		.addFunction("task_get", (void (CommandTestImpl::*)(S_INT_32))&CommandTestImpl::task_get)
		.addFunction("task_submit", (void (CommandTestImpl::*)(S_INT_32))&CommandTestImpl::task_submit)
		.addFunction("task_giveup", (void (CommandTestImpl::*)(S_INT_32))&CommandTestImpl::task_giveup)
		.endClass()
		.endNamespace();
}
