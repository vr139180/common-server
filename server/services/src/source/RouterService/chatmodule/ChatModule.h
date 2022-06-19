#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__

#include <boost/atomic.hpp>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include "config/RouterConfig.h"

class ChatModule
{
private:
	ChatModule();
public:
	static ChatModule& instance();

	void init_chatmodule(S_INT_64 myiid);

	void process_chat_msg(S_UINT_16 proiid, BasicProtocol* pro);

protected:
	void on_customchannelid_req(S_INT_64 sidgid, S_INT_64 slottoken);
	void on_userchannel_active(BasicProtocol* pro);
	void on_user_say_somthing(BasicProtocol* pro);

protected:
	//�½��µı��
	S_INT_64 new_custom_channelid();

	//channeldid hash ��chat����
	int channelid_to_chathash( int type, S_INT_64 channelid);

private:
	ChatHashPlot	chash_plot_;

	//sid��Ϊcustom channel id������
	S_INT_64				channel_seed_;
	boost::atomic<S_INT_64>	iids_;
};

#endif //__CHATMODULE_H__
