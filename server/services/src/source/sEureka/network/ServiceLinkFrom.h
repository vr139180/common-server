#ifndef __SERVICELINKFROM_H__
#define __SERVICELINKFROM_H__

#include <cmsLib/prolib/core_type.h>
#include <gameLib/service_type.h>
#include <gameLib/eureka/ServiceNodeInfo.h>
#include <gameLib/commons/NetLinkFromBase.h>
#include "network/EurekaSession.h"

class ServiceLinkFrom : public NetLinkFromBase<EurekaSession>
{
public:
	ServiceLinkFrom();

	void set_node(ServiceNodeInfo* pnode) { node_.reset(pnode); }
	ServiceNodeInfo* get_node() { return node_.get(); }

	virtual void reset();
	virtual void registinfo_tolog( bool bregist =true);
	virtual void force_linkclose();

public:
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);

	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_INT_64 get_token() { return node_->token; }

protected:
	std::auto_ptr<ServiceNodeInfo>	node_;
};

#endif	//__SERVICELINKFROM_H__
