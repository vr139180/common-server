#include "gameLib/eureka/EurekaClusterClient.h"

#include <cmsLib/system/CommandBase.h>
#include "gameLib/protobuf/Proto_all.h"
#include <gameLib/LogExt.h>

USE_PROTOCOL_NAMESPACE

void EurekaClusterClient::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF, &EurekaClusterClient::on_eurekaupdate_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_ACK, &EurekaClusterClient::on_service_subscribe_ack, this);
	REGISTERMSG(ERK_PROTYPE::SVR_GATEBINDHOME_ACK, &EurekaClusterClient::on_gatebindhome_ack, this);
}

void EurekaClusterClient::on_eurekaupdate_ntf(BasicProtocol* message, bool& autorelease)
{
	Erk_EurekaUpdate_ntf *ntf = dynamic_cast<Erk_EurekaUpdate_ntf*>(message);

	//��������
	for (int ind = 0; ind < ntf->online_size(); ++ind)
	{
		const EurekaServerNode& node = ntf->online(ind);
		if (is_eurekanode_exist(node.iid()))
			continue;

		//���������ӵĽڵ�
		EurekaNodeInfo* pinfo = new EurekaNodeInfo();
		pinfo->iid = node.iid();
		pinfo->token = node.token();
		pinfo->ip = node.ip();
		pinfo->port = node.port();

		eureka_nodes_[pinfo->iid] = pinfo;

		//eureka�Ѿ�����
		if (is_eurekanode_connected(pinfo->iid))
			continue;

		//ȫ�µ�eureka���������ӻ���
		EurekaClusterLink* pconn = 0;
		//�������ӽڵ�
		if (free_links_.size() == 0)
		{
			EurekaClusterLink* pc = new EurekaClusterLink(this, pinfo->clone());
			clusterlinks_.push_back(pc);
			pconn = pc;
		}
		else
		{
			std::set<EurekaClusterLink*>::iterator fiter = free_links_.begin();
			pconn = (*fiter);
			free_links_.erase(fiter);

			pconn->reset(pinfo->clone());
		}

		wait_links_.insert(pconn);
		logDebug(out_runtime, "recv a new eureka node:%lld wait to connecting", pconn->get_iid());
	}

	//ɾ�����ߵķ��񣬵��ǲ�ֱ�ӶϿ����ӡ�ͨ����������ʱ���Զ�ά��
	for (int ind = 0; ind < ntf->offline_size(); ++ind)
	{
		S_INT_64 sid = ntf->offline(ind);
		boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator fiter = eureka_nodes_.find(sid);
		if (fiter == eureka_nodes_.end())
			continue;

		EurekaNodeInfo* pp = fiter->second;
		eureka_nodes_.erase(fiter);

		delete pp;
	}
}

void EurekaClusterClient::on_service_subscribe_ack(BasicProtocol* message, bool& autorelease)
{
	Erk_ServiceSubscribe_ack* ack = dynamic_cast<Erk_ServiceSubscribe_ack*>(message);

	NETSERVICE_TYPE type = (NETSERVICE_TYPE)ack->svr_type();
	SERVICENODE_TYPE& s_nodes = get_servicenodes_by_type( type);

	std::list<ServiceNodeInfo*> newiids;
	//���߷���������Ϣ
	for (int ind = 0; ind < ack->online_size(); ++ind)
	{
		const ServerNode& node = ack->online(ind);
		SERVICENODE_TYPE::iterator fiter = s_nodes.find(node.iid());
		if (fiter != s_nodes.end())
			continue;

		//��������
		ServiceNodeInfo* ps = new ServiceNodeInfo();
		ps->iid = node.iid();
		ps->token = node.token();
		ps->ip = node.ip();
		ps->port = node.port();
		if (node.exts_size() > 0)
		{
			const google::protobuf::Map<std::string, std::string>& kvs = node.exts();
			for (google::protobuf::Map<std::string, std::string>::const_iterator xiter = kvs.cbegin();
				xiter != kvs.cend(); ++xiter)
			{
				ps->extparams[xiter->first] = xiter->second;
			}
		}

		s_nodes[ps->iid] = ps;

		newiids.push_back(ps);
	}

	//���߷���
	std::list<S_INT_64>	deliids;
	for (int ind = 0; ind < ack->offline_size(); ++ind)
	{
		S_INT_64 sid = ack->offline(ind);
		SERVICENODE_TYPE::iterator fiter = s_nodes.find(sid);
		if (fiter == s_nodes.end())
			continue;

		deliids.push_back(sid);

		ServiceNodeInfo* pp = fiter->second;
		s_nodes.erase(fiter);

		delete pp;
	}

	if (app_proxy_)
	{
		app_proxy_->mth_notify_servicenode_new(type, newiids, deliids);
	}

	//��һ��ע�ᣬ����֮�� �ظ�ȷ��Э��
	if (cur_state_ == EurekaState::Eureka_Registed)
	{
		Erk_ServiceRegist_Confirm *confirm = new Erk_ServiceRegist_Confirm();
		confirm->set_iid(this->service_iid_);
		confirm->set_token(this->token_);

		this->send_mth_protocol(confirm);
		cur_state_ = EurekaState::Eureka_Done;
		
		if (app_proxy_)
			app_proxy_->mth_service_registed(service_iid_);
	}
}

void EurekaClusterClient::on_gatebindhome_ack(BasicProtocol* message, bool& autorelease)
{
	logDebug(out_runtime, "recv gate bind home response.........");
	if (app_proxy_ == 0)
		return;

	Svr_GateBindHome_ack* ack = dynamic_cast<Svr_GateBindHome_ack*>(message);
	ServiceNodeInfo *pnode = 0;
	if (ack->result() == 0)
	{
		pnode = new ServiceNodeInfo();
		pnode->type = NETSERVICE_TYPE::ERK_SERVICE_HOME;
		pnode->iid = ack->homeiid();
		pnode->token = ack->hometoken();
		pnode->ip = ack->homeip();
		pnode->port = ack->homeport();
	}

	app_proxy_->on_mth_gatebindhome_ack(ack->result() == 0, pnode, ack->gateiid(), ack->bindtoken());
}