#include "network/EurekaLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include "sEurekaApp.h"

EurekaLinkTo::EurekaLinkTo(EurekaNodeInfo* p):LinkToBase()
,node_(p)
, fail_num_(0)
{
}

EurekaLinkTo::~EurekaLinkTo()
{
	reset(0);
}

void EurekaLinkTo::reset(EurekaNodeInfo* pnode)
{
	fail_num_ = 0;
	node_.reset(pnode);
	force_close();
}

void EurekaLinkTo::force_linkclose()
{
	force_close();
}

void EurekaLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_net, "++++++me(sEureka) try to connect to sEureka(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);
#endif

	connect_to(node_->ip.c_str(), node_->port);
}

void EurekaLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_net, "------me(sEureka) cant connect to sEureka(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);
#endif

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void EurekaLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_net, "++++++me(sEureka) connected to sEureka(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);
#endif

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void EurekaLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>(
		boost::bind( &EurekaLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void EurekaLinkTo::on_recv_protocol_netthread( S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
	
	if (proiid == ERK_PROTYPE::ERK_EUREKABIND_ACK)
    {
        PRO::Erk_EurekaBind_ack* ack =dynamic_cast<Erk_EurekaBind_ack*>(pro);

        bool bauth =(ack->result() == 0);
        SystemCommand2<bool>* cmd =new SystemCommand2<bool>(
			boost::bind( &EurekaLinkTo::on_authed, this, boost::placeholders::_1), bauth);
        svrApp.regist_syscmd( cmd);
    }
    else
    {
		NETCMD_FUN_MAP fun = boost::bind(
			&EurekaClusterCtrl::NetProcessMessage, svrApp.get_eurekactrl(), boost::placeholders::_1, boost::placeholders::_2,(int)proiid);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		svrApp.regist_syscmd(pcmd);
    }
}

void EurekaLinkTo::on_connected( bool success)
{
    if( success)
    {
		EurekaNodeInfo& myself = svrApp.get_eurekactrl()->get_myself();

		//发送绑定请求
        PRO::Erk_EurekaBind_req* req = new PRO::Erk_EurekaBind_req();
		req->set_iid(myself.iid);
		req->set_token( myself.token);
		req->set_ip(myself.ip);
		req->set_port(myself.port);

		this->send_protocol( req);
    }
    else
    {
		++fail_num_;
		//连接不上eureka 触发maintnce检测
		svrApp.get_eurekactrl()->on_cantconnect_with_linkto(this);
    }
}

void EurekaLinkTo::on_authed( bool success)
{
    if( success)
    {
        logInfo( out_boot, ">>>>>> me(sEureka) auth to sEureka[iid:%ld ip:%s port:%d]!!!!", 
			node_->iid, node_->ip.c_str(), node_->port);
        this->set_authed( true);
		svrApp.get_eurekactrl()->on_authed_with_linkto(this);
	}
    else
    {
#ifdef EUREKA_DEBUGINFO_ENABLE
        logInfo( out_boot, "<<<<<< me(sEureka) auth to sEureka[iid:%ld ip:%s port:%d] failed!!!!", 
			node_->iid, node_->ip.c_str(), node_->port);
#endif
        
		++fail_num_;
		//先断开连接
		this->force_linkclose();
		//注册绑定失败警告
		//S_INT_64 myiid = svrApp.get_eurekactrl()->get_myself().iid;
		//svrApp.get_eurekactrl()->warning_eurekabind_failed(myiid, node_->iid);
    }

}

void EurekaLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
        logInfo( out_boot, "<<<<<< me(sEureka)(authed) lost connection to sEureka[iid:%ld ip:%s port:%d]", 
			node_->iid, node_->ip.c_str(), node_->port);
    }
	else
	{
		logInfo(out_boot, "<<<<<< me(sEureka)(wait auth) lost connection to sEureka[iid:%ld ip:%s port:%d]",
			node_->iid, node_->ip.c_str(), node_->port);
	}

	svrApp.get_eurekactrl()->on_disconnected_with_linkto(this);

    //this->reset( 0);
}

void EurekaLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* EurekaLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}
