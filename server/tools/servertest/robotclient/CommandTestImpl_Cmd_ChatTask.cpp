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

	CString str1;
	str1.Format("可获取任务数 size:%d\r\n", ack->task_iids_size());
	*pRetMsg += str1;
	for (int ii = 0; ii < ack->task_iids_size(); ++ii)
	{
		str1.Format("\t task iid:%d \r\n", ack->task_iids(ii));
		*pRetMsg += str1;
	}
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

	CString str1;
	str1.Format("我的任务信息 group:%d task:%d\r\n", ack->groups().groups_size(), ack->tasks().items_size());
	*pRetMsg += str1;
	const PRO::DBUserTaskGroups& gs = ack->groups();
	for (int ii = 0; ii < gs.groups_size(); ++ii)
	{
		str1.Format("\t task group iid:%d \r\n", gs.groups(ii).task_group());
		*pRetMsg += str1;
	}
	const PRO::DBUserTasks& ts = ack->tasks();
	for (int ii = 0; ii < ts.items_size(); ++ii)
	{
		str1.Format("\t task iid:%d \r\n", ts.items(ii).task_iid());
		*pRetMsg += str1;
	}
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

	CString str1;
	str1.Format("获取任务 taskid:%d result:%d\r\n", ack->task_iid(), ack->result());
	*pRetMsg += str1;
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

	CString str1;
	str1.Format("提交任务 taskid:%d result:%d\r\n", ack->task_iid(), ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::on_task_obtainreward_ntf(BasicProtocol* pro, CString* pRetMsg)
{
	Task_ObtainReward_ntf* ack = dynamic_cast<Task_ObtainReward_ntf*>(pro);
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
