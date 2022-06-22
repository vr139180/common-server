#ifndef __EUREKALINKFROM_H__
#define __EUREKALINKFROM_H__

#include <gameLib/eureka/EurekaNodeInfo.h>
#include <gameLib/commons/NetLinkFromBase.h>

#include "network/IEurekaNodeLink.h"
#include "network/EurekaSession.h"

class EurekaLinkFrom : public NetLinkFromBase<EurekaSession>, public IEurekaNodeLink
{
public:
	EurekaLinkFrom();
	EurekaLinkFrom(EurekaNodeInfo* pnode);

	void set_node(EurekaNodeInfo* pnode) { node_.reset(pnode); }

	virtual void reset();

	virtual void registinfo_tolog( bool bregist =true);


public:
	//-----------------NetSessionBindEvent-------------------------------
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);

	//---------------IEurekaNodeLink-----------------------------
	virtual void send_protocol(BasicProtocol* pro) { NetLinkFromBase<EurekaSession>::send_protocol(pro); }
	virtual bool is_ready() { return NetLinkFromBase::is_valid(); }
	virtual bool is_link_to() { return false; }
	virtual void force_linkclose();

	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_INT_64 get_token() { return node_->token; }
	virtual void heart_beat();

protected:
	std::shared_ptr<EurekaNodeInfo>	node_;
};

#endif	//__EUREKALINKFROM_H__
