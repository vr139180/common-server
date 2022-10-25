#ifndef __SERVICELINKFROM_H__
#define __SERVICELINKFROM_H__

#include <cmsLib/core_type.h>
#include <gameLib/service_type.h>
#include <gameLib/eureka/ServiceNodeInfo.h>
#include <gameLib/commons/NetLinkFromBase.h>
#include "network/EurekaSession.h"

class ServiceLinkFrom : public NetLinkFromBase<EurekaSession>
{
public:
	ServiceLinkFrom();

	void set_node(ServiceNodeInfo* pnode);
	ServiceNodeInfo* get_node() { return node_.get(); }

	virtual void reset();
	virtual void registinfo_tolog( bool bregist =true);
	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	void send_to_service(BasicProtocol* msg);

public:
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);

	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_INT_64 get_token() { return node_->token; }

protected:
	std::shared_ptr<ServiceNodeInfo>	node_;

	//协议头缺省
	SProtocolHead	s_head_;
};

#endif	//__SERVICELINKFROM_H__
