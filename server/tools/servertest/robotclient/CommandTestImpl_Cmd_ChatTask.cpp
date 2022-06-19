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
