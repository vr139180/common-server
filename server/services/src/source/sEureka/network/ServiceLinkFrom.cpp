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

#include "network/ServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

ServiceLinkFrom::ServiceLinkFrom():NetLinkFromBase<EurekaSession>()
{
	this->init_protocolhead();
}

void ServiceLinkFrom::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_type_ = (S_INT_8)node_.type;
}

void ServiceLinkFrom::set_node(ServiceNodeInfo* pnode)
{
	node_ = *pnode;
	s_head_.to_type_ = (S_INT_8)node_.type;

	set_linkbase_info(node_.iid, node_.token);
}

void ServiceLinkFrom::send_to_service(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	SProtocolHead& head = pro->write_head();

	NetLinkFromBase<EurekaSession>::send_protocol(pro);
}

void ServiceLinkFrom::reset()
{
	NetLinkFromBase<EurekaSession>::reset();
}

void ServiceLinkFrom::force_linkclose()
{
	force_close();
}

void ServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<ServiceLinkFrom>* cmd = new SystemCommand<ServiceLinkFrom>( boost::bind(
			&ServiceRegisterCtrl::on_mth_disconnected_with_service, svrApp.get_servicectrl(), this));
	svrApp.regist_syscmd( cmd);
}

void ServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	//缺省处理都在servie线程池中进行
	NETCMD_FUN_MAP fun = boost::bind(
		&ServiceRegisterCtrl::NetProcessMessage, svrApp.get_servicectrl(), boost::placeholders::_1, boost::placeholders::_2);

	NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
	svrApp.regist_netcmd(pcmd);
}

void ServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if (bregist)
	{
		logInfo(out_runtime, ">>>>>> Service[iid:%ld type:%s ip:%s port:%d] regist to me(sEureka)",
			node_.iid, NetServiceType::to_string(node_.type).c_str(), node_.ip.c_str(), node_.port);
	}
	else 
	{
		logInfo(out_runtime, "<<<<<< Service[iid:%ld type:%s ip:%s port:%d] disconnect from me(sEureka)",
			node_.iid, NetServiceType::to_string(node_.type).c_str(), node_.ip.c_str(), node_.port);
	}
}
