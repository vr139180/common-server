#ifndef __CHATLINKERHOLDER_H__
#define __CHATLINKERHOLDER_H__

#include <gameLib/commons/LinkFromHolder.h>
#include "network/ChatServiceLinkFrom.h"

class ChatLinkerHolder : public LinkFromHolder<ChatServiceLinkFrom>
{
	typedef LinkFromHolder<ChatServiceLinkFrom> base;
public:
	ChatLinkerHolder();

	virtual void uninit_holder();

	virtual void return_freelink(ChatServiceLinkFrom* link);
	virtual ChatServiceLinkFrom* regist_onlinelink(ChatServiceLinkFrom* link);

	void send_mth_protocol(int chathash, BasicProtocol* pro);

protected:
	boost::unordered_map<int, ChatServiceLinkFrom*>	hash_service_;
};

#endif //__CHATLINKERHOLDER_H__
