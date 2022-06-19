#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::friend_invite(S_INT_64 toiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Frd_FriendInvite_req *act = new Frd_FriendInvite_req();
	act->set_invite_iid(toiid);
	send_to_gts(act);
}

void CommandTestImpl::on_friend_invite_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Frd_FriendInvite_ack *ack = dynamic_cast<Frd_FriendInvite_ack*>(pro);
	CString str1;
	str1.Format("friend- invite friend result:%d \r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::friend_inviteconfirm(S_INT_64 iid, bool agree)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Frd_InviteConfirm_req *act = new Frd_InviteConfirm_req();
	act->set_iid(iid);
	act->set_agree(agree);

	send_to_gts(act);
}

void CommandTestImpl::on_friend_inviteconfirm_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Frd_InviteConfirm_ack *ack = dynamic_cast<Frd_InviteConfirm_ack*>(pro);
	CString str1;
	str1.Format("friend- invite confirm friend result:%d \r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::friend_delete(S_INT_64 iid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Frd_FriendDelete_req *act = new Frd_FriendDelete_req();
	act->set_friendiid(iid);

	send_to_gts(act);
}

void CommandTestImpl::on_friend_delete_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Frd_FriendDelete_ack *ack = dynamic_cast<Frd_FriendDelete_ack*>(pro);
	CString str1;
	str1.Format("friend- delete friend result:%d \r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::friend_get(S_INT_64 invite, S_INT_32 find)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Frd_FriendList_req *act = new Frd_FriendList_req();
	act->set_last_invite(invite);
	act->set_friendindex(find);
	act->set_next(true);
	act->set_num(20);
	act->set_nickname("xxxxx");

	send_to_gts(act);
}

void CommandTestImpl::on_friend_get_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Frd_FriendList_ack *ack = dynamic_cast<Frd_FriendList_ack*>(pro);
	CString str1;
	str1.Format("friend- get friend invite:%d friend:%d \r\n", ack->invites_size(), ack->friends_size());
	*pRetMsg += str1;
	for (int ii = 0; ii < ack->invites_size(); ++ii)
	{
		const FriendInviteItem& fi = ack->invites(ii);
		str1.Format("\t invite:%lld from:%lld nick:%s\r\n", fi.iid(), fi.from_iid(), fi.nickname().c_str());
		*pRetMsg += str1;
	}
	for (int ii = 0; ii < ack->friends_size(); ++ii)
	{
		const FriendRelation& fi = ack->friends(ii);
		str1.Format("\t frd:%lld friend:%lld nick:%s online:%d\r\n", fi.iid(), fi.friendiid(), fi.nickname().c_str(), fi.online());
		*pRetMsg += str1;
	}
}