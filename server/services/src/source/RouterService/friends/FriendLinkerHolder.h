#ifndef __FRIENDLINKERHOLDER_H__
#define __FRIENDLINKERHOLDER_H__

#include <gameLib/commons/LinkFromHolder.h>
#include "network/FriendServiceLinkFrom.h"

class FriendLinkerHolder : public LinkFromHolder<FriendServiceLinkFrom>
{
	typedef LinkFromHolder<FriendServiceLinkFrom> base;
public:
	FriendLinkerHolder();

	virtual void uninit_holder();

	virtual void return_freelink(FriendServiceLinkFrom* link);
	virtual FriendServiceLinkFrom* regist_onlinelink(FriendServiceLinkFrom* link);

	void send_mth_protocol(int mailhash, BasicProtocol* pro);
	void send_mth_protocol_circle(BasicProtocol* pro);

protected:
	boost::unordered_map<int, FriendServiceLinkFrom*>	hash_service_;
	std::vector<FriendServiceLinkFrom*>					friend_vector_;

	int		cur_friend_service_;
};

#endif //__FRIENDLINKERHOLDER_H__
