#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;

void CommandTestImpl::chat_say(int t, S_INT_64 cid, const char* content)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Chat_UserChannels_active *act = new Chat_UserChannels_active();
	ChatChannelInfo* ci = act->add_channels();
	ci->set_type((ChatChannelType)t);
	ci->set_channeldid(cid);
	send_to_gts(act);

	Chat_UserMsg_say *say = new Chat_UserMsg_say();
	ci = say->mutable_channel();
	ci->set_type((ChatChannelType)t);
	ci->set_channeldid(cid);
	say->set_nickname("test");
	say->set_msg_content(content);

	send_to_gts(say);
}

void CommandTestImpl::on_chat_chatmsg_ntf(BasicProtocol* pro, CString* pRetMsg)
{
	Chat_ChatMsg_ntf* ack = dynamic_cast<Chat_ChatMsg_ntf*>(pro);

	CString str1;
	const ChatChannelInfo& cci = ack->channel();
	str1.Format("聊天消息 type:%d channel:%lld \r\n", cci.type(), cci.channeldid());
	*pRetMsg += str1;
	const ChatMessageItems& its = ack->msgs();
	for (int ii = 0; ii < its.msgs_size(); ++ii)
	{
		const ChatMessageItem& it = its.msgs(ii);
		str1.Format("\t who:%s say:%s \r\n", it.send_nickname().c_str(), it.msg_content().c_str());
		*pRetMsg += str1;
	}

}

void CommandTestImpl::task_waitlist()
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_WaitList_req *req = new Task_WaitList_req();
	send_to_gts( req);
}

void CommandTestImpl::on_task_waitlist_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_WaitList_ack* ack = dynamic_cast<Task_WaitList_ack*>(pro);
}

void CommandTestImpl::task_mytasks()
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_MyTaskList_req *req = new Task_MyTaskList_req();
	send_to_gts(req);
}

void CommandTestImpl::on_task_mytasks_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_MyTaskList_ack* ack = dynamic_cast<Task_MyTaskList_ack*>(pro);
}

void CommandTestImpl::task_get(S_INT_32 taskiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_GetTask_req *req = new Task_GetTask_req();
	req->set_task_iid(taskiid);
	send_to_gts(req);
}

void CommandTestImpl::on_task_get_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_GetTask_ack* ack = dynamic_cast<Task_GetTask_ack*>(pro);
}

void CommandTestImpl::task_submit(S_INT_32 taskiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_SubmitTask_req *req = new Task_SubmitTask_req();
	req->set_task_iid(taskiid);
	send_to_gts(req);
}

void CommandTestImpl::on_task_submit_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_SubmitTask_ack* ack = dynamic_cast<Task_SubmitTask_ack*>(pro);
}

void CommandTestImpl::task_obtainreward(S_INT_32 taskiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_ObtainReward_req *req = new Task_ObtainReward_req();
	req->set_task_iid(taskiid);
	send_to_gts(req);
}

void CommandTestImpl::on_task_obtainreward_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_ObtainReward_ack* ack = dynamic_cast<Task_ObtainReward_ack*>(pro);
}

void CommandTestImpl::task_giveup(S_INT_32 taskiid)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Task_GiveupTask_req *req = new Task_GiveupTask_req();
	req->set_task_iid(taskiid);
	send_to_gts(req);
}

void CommandTestImpl::on_task_giveup_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Task_GiveupTask_ack* ack = dynamic_cast<Task_GiveupTask_ack*>(pro);
}
