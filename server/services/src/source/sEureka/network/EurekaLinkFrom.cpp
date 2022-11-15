// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "network/EurekaLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

EurekaLinkFrom::EurekaLinkFrom():NetLinkFromBase<EurekaSession>()
{
	this->init_protocolhead();
}

EurekaLinkFrom::EurekaLinkFrom(EurekaNodeInfo pnode) : NetLinkFromBase<EurekaSession>()
{
	this->node_ = pnode;
	this->init_protocolhead();
}

void EurekaLinkFrom::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
}

void EurekaLinkFrom::send_to_eureka(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	NetLinkFromBase<EurekaSession>::send_protocol(pro);
}

void EurekaLinkFrom::reset()
{
	NetLinkFromBase<EurekaSession>::reset();
}

void EurekaLinkFrom::force_linkclose()
{
	force_close();
}

void EurekaLinkFrom::set_node(EurekaNodeInfo pnode)
{
	node_ = pnode;
	set_linkbase_info(node_.iid, node_.token);
}

void EurekaLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<EurekaLinkFrom>* cmd = new SystemCommand<EurekaLinkFrom>( 
			boost::bind( &sEurekaApp::on_disconnected_with_linkfrom, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void EurekaLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	NETCMD_FUN_MAP fun = boost::bind(&EurekaClusterCtrl::NetProcessMessage, svrApp.get_eurekactrl(), 
		boost::placeholders::_1, boost::placeholders::_2);

	NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
	svrApp.regist_syscmd(pcmd);
}

void EurekaLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo(out_runtime, ">>>>>> sEureka[%d] regist to me(sEureka)", node_.iid);
	else
		logInfo(out_runtime, "<<<<<< sEureka[%d] disconnect from me(sEureka)", node_.iid);
}

void EurekaLinkFrom::heart_beat()
{
	NetLinkFromBase<EurekaSession>::heart_beat();
}
