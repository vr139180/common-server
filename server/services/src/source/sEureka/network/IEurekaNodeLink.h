#ifndef __IEUREKANODELINK_H__
#define __IEUREKANODELINK_H__

#include <cmsLib/prolib/core_type.h>

class IEurekaNodeLink
{
public:
	IEurekaNodeLink() {}

	virtual S_INT_64 get_iid() = 0;
	virtual S_INT_64 get_token() = 0;

	virtual void send_protocol(BasicProtocol* pro) = 0;
	virtual bool is_ready() = 0;

	virtual bool is_link_to() = 0;

	virtual void force_linkclose() = 0;

	virtual void heart_beat() = 0;
};

#endif //__IEUREKANODELINK_H__