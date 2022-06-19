#ifndef __FRIENDMODULE_H__
#define __FRIENDMODULE_H__

#include <boost/atomic.hpp>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include "config/RouterConfig.h"

class FriendModule
{
private:
	FriendModule();
public:
	static FriendModule& instance();

	void init_friendmodule(S_INT_64 myiid);

	void process_friend_msg(S_UINT_16 proiid, BasicProtocol* pro);

	//receiver hash µ½mail·þÎñ
	int user_to_frdhash(S_INT_64 useriid);

private:
	FriendHashPlot	fhash_plot_;
};

#endif //__FRIENDMODULE_H__
