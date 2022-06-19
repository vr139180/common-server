#include "chatmodule/ChatLinkerHolder.h"

ChatLinkerHolder::ChatLinkerHolder():base()
{
}

void ChatLinkerHolder::uninit_holder()
{
	hash_service_.clear();

	base::uninit_holder();
}

void ChatLinkerHolder::return_freelink(ChatServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	//hash services
	int cid = link->get_chathash();
	if (cid != -1)
	{
		//��Զ����ͬchathash��chat�����л�ʱ��ɵ�
		boost::unordered_map<int, ChatServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			ChatServiceLinkFrom* p = fiter->second;
			if (p == link)
				hash_service_.erase(fiter);
		}
	}
	
	online_links_.erase(link->get_iid());
	free_links_.insert(link);
}

ChatServiceLinkFrom* ChatLinkerHolder::regist_onlinelink(ChatServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_[link->get_iid()] = link;

	int cid = link->get_chathash();
	if (cid != -1)
	{
		//��Զ����ͬchathash��chat�����л�ʱ��ɵ�
		boost::unordered_map<int, ChatServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			ChatServiceLinkFrom* p = fiter->second;
			//iid��ı�ʾ���������ģ��滻ǰ�������ģ�ÿ��chathash����ֻ�ܴ���һ��
			if (link->get_iid() > p->get_iid())
				hash_service_[cid] = link;
		}
		else
		{
			hash_service_[cid] = link;
		}
	}

	return link;
}

void ChatLinkerHolder::send_mth_protocol(int chathash, BasicProtocol* pro)
{
	ThreadLockWrapper guard(lock_);

	boost::unordered_map<int, ChatServiceLinkFrom*>::iterator fiter = hash_service_.find(chathash);
	if (fiter == hash_service_.end())
	{
		delete pro;
		return;
	}

	ChatServiceLinkFrom* p = fiter->second;
	p->send_protocol(pro);
}
