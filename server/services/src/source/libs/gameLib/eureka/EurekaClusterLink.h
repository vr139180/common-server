#ifndef __EUREKACLUSTERLINK_H__
#define __EUREKACLUSTERLINK_H__

#include <cmsLib/net/LinkToBase.h>

#include <gameLib/eureka/EurekaNodeInfo.h>

class EurekaClusterClient;

class EurekaClusterLink : public LinkToBase
{
public:
	EurekaClusterLink(EurekaClusterClient* p, EurekaNodeInfo* info);
	virtual ~EurekaClusterLink();

	void connect();
	void reset(EurekaNodeInfo* pnode);

	virtual void force_linkclose();

	virtual std::string get_ext_bykey(const char* key) { return ""; }

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	S_INT_64 get_iid() { return node_->iid; }
	EurekaNodeInfo* clone_node() {
		if (node_.get() == 0) return 0;
		return node_->clone();
	}

	void send_to_eureka(BasicProtocol* msg);

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(NetProtocol* pro);

public:
	void on_connected(bool success);
	void on_disconnected();
	void on_regist_result(NetProtocol* message, bool& autorelease);
	void on_bind_result(bool success);

protected:
	std::unique_ptr<EurekaNodeInfo>	node_;

	//协议头
	SProtocolHead					s_head_;

	//连接错误次数
	int		fail_num_;
	EurekaClusterClient*			parent_;
};

#endif //__EUREKACLUSTERLINK_H__