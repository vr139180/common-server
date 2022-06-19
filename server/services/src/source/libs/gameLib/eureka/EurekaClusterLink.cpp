#include "gameLib/eureka/EurekaClusterLink.h"

#include "gameLib/eureka/EurekaClusterClient.h"
#include "gameLib/protobuf/Proto_all.h"
#include <gameLib/LogExt.h>

USE_PROTOCOL_NAMESPACE

EurekaClusterLink::EurekaClusterLink(EurekaClusterClient* p, EurekaNodeInfo* info) :LinkToBase()
, fail_num_(0)
, parent_(p)
, node_( info)
{
}

EurekaClusterLink::~EurekaClusterLink()
{
	reset(0);
}

void EurekaClusterLink::reset(EurekaNodeInfo* pnode)
{
	fail_num_ = 0;
	node_.reset(pnode);
	force_close();
}

void EurekaClusterLink::force_linkclose()
{
	force_close();
}

void EurekaClusterLink::connect()
{
	if (is_connected() || is_connecting())
		return;

	logDebug(out_net, "++++++++me try to connect to sEureka(iid:%ld ip:%s port:%d)++++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void EurekaClusterLink::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logDebug(out_net, "---------me cant connect to sEureka(iid:%ld ip:%s port:%d)---------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaClusterLink::on_connected, this, boost::placeholders::_1), false);

	parent_->regist_command(cmd);
}

void EurekaClusterLink::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logDebug(out_net, "++++++++me connected to sEureka(iid:%ld ip:%s port:%d)++++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaClusterLink::on_connected, this, boost::placeholders::_1), true);

	parent_->regist_command(cmd);
}

void EurekaClusterLink::on_connect_lost_netthread()
{
	SystemCommand<bool>* cmd = new SystemCommand<bool>(
		boost::bind(&EurekaClusterLink::on_disconnected, this));

	parent_->regist_command(cmd);
}

void EurekaClusterLink::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);

	if (proiid == ERK_PROTYPE::ERK_SERVICEREGIST_ACK)
	{
		NETCMD_FUN_MAP fun = boost::bind(
			&EurekaClusterLink::on_regist_result, this, boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		parent_->regist_command(pcmd);
	}
	else if (proiid == ERK_PROTYPE::ERK_SERVICEBIND_ACK)
	{
		Erk_ServiceBind_ack* ack = dynamic_cast<Erk_ServiceBind_ack*>(pro);

		bool bsucc = (ack->result() == 0);
		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&EurekaClusterLink::on_bind_result, this, boost::placeholders::_1), bsucc);

		parent_->regist_command(cmd);
	}
	else
	{
		NETCMD_FUN_MAP2 fun = boost::bind(
			&EurekaClusterClient::NetProcessMessage, parent_, 
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

		NetCommand *pcmd = new NetCommand( p_msg.release(), fun, (int)proiid);
		parent_->regist_command(pcmd);
	}
}

void EurekaClusterLink::on_connected(bool success)
{
	if (success)
	{
		if (parent_->is_registed())
		{
			//已注册，则进行绑定
			Erk_ServiceBind_req* req = new Erk_ServiceBind_req();

			req->set_svr_type(parent_->get_svrtype());
			req->set_iid(parent_->get_myiid());
			req->set_token( parent_->get_token());

			this->send_protocol(req);
		}
		else
		{
			Erk_ServiceRegist_req* req = new Erk_ServiceRegist_req();

			req->set_svr_type(parent_->get_svrtype());
			req->set_ip(parent_->get_myip());
			req->set_port( parent_->get_myport());
			
			const EurekaServerExtParam& et = parent_->get_extps();
			if (et.size() > 0)
			{
				google::protobuf::Map<std::string, std::string>& ext = *(req->mutable_exts());
				for (EurekaServerExtParam::const_iterator xiter = et.cbegin(); xiter != et.cend(); ++xiter)
				{
					ext[xiter->first] = xiter->second;
				}
			}

			this->send_protocol(req);
		}
	}
	else
	{
		++fail_num_;
		parent_->on_link_disconnected(this);
	}
}

void EurekaClusterLink::on_disconnected()
{
	//need notify server, connection error
	if (this->is_authed())
	{
		logInfo(out_net, "me(auth) lost connection to sEureka[iid:%ld ip:%s port:%d]",
			node_->iid, node_->ip.c_str(), node_->port);
	}
	else
	{
		logInfo(out_net, "me(wait auth) lost connection to sEureka[iid:%ld ip:%s port:%d]",
			node_->iid, node_->ip.c_str(), node_->port);
	}

	parent_->on_link_disconnected(this);
}

void EurekaClusterLink::on_regist_result(BasicProtocol* message, bool& autorelease)
{
	Erk_ServiceRegist_ack* ack = dynamic_cast<Erk_ServiceRegist_ack*>(message);
	bool success = (ack->result() == 0);

	if( success)
	{
		logInfo(out_net, "me regist to sEureka[iid:%ld ip:%s port:%d] success",
			node_->iid, node_->ip.c_str(), node_->port);

		this->set_authed(true);

		//这个eureka对应的 iid, token. 第一个几点只知道ip,port不知道 iid,token
		this->node_->iid = ack->eurekaiid();
		this->node_->token = ack->eurekatoken();

		//保存系统分配的serviceiid，token
		parent_->set_myinfo(ack->iid(), ack->token());

		parent_->on_link_regist_result(this);
	}
	else
	{
		logInfo(out_boot, "me to sEureka[iid:%ld ip:%s port:%d] regist failed",
			node_->iid, node_->ip.c_str(), node_->port);

		++fail_num_;
		//eureka服务器会主动断开连接
		//this->force_close();
	}
}

void EurekaClusterLink::on_bind_result(bool success)
{
	if (success)
	{
		logInfo(out_boot, "me bind to sEureka[iid:%ld ip:%s port:%d] success",
			node_->iid, node_->ip.c_str(), node_->port);
		this->set_authed(true);

		parent_->on_link_bind_result(this);
	}
	else
	{
		logInfo(out_boot, "me bind to sEureka[iid:%ld ip:%s port:%d] failed",
			node_->iid, node_->ip.c_str(), node_->port);

		//eureka服务器会主动断开连接
		//this->force_close();
		++fail_num_;
	}
}
