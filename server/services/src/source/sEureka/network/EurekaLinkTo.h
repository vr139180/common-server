#ifndef __EUREKALINKTO_H__
#define __EUREKALINKTO_H__

#include <cmsLib/net/LinkToBase.h>
#include <gameLib/eureka/EurekaNodeInfo.h>

#include "network/IEurekaNodeLink.h"

class EurekaLinkTo : public LinkToBase, public IEurekaNodeLink
{
public:
	EurekaLinkTo(EurekaNodeInfo* p);
	virtual ~EurekaLinkTo();

	//重置放入代用缓存
	void reset( EurekaNodeInfo* pnode);

	virtual void connect();
	virtual void send_protocol(BasicProtocol* pro) { LinkToBase::send_protocol(pro); }
	virtual bool is_ready() { return LinkToBase::is_authed(); }
	virtual std::string get_ext_bykey(const char* key) { return ""; }

	virtual void force_linkclose();

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual BasicProtocol* get_livekeep_msg();

	//---------------IEurekaNodeLink-----------------------------
	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_INT_64 get_token() { return node_->token; }
	virtual bool is_link_to() { return true; }

	virtual void heart_beat();

public:
    void on_connected( bool success);
    void on_authed( bool success);
    void on_disconnected();

protected:
	std::auto_ptr<EurekaNodeInfo>	node_;

	//连接错误次数
	int		fail_num_;
};

#endif	//__EUREKALINKTO_H__
