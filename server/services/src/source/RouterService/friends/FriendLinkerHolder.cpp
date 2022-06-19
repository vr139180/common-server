#include "friends/FriendLinkerHolder.h"

FriendLinkerHolder::FriendLinkerHolder():base()
, cur_friend_service_(0)
{
}

void FriendLinkerHolder::uninit_holder()
{
	friend_vector_.clear();
	hash_service_.clear();

	base::uninit_holder();
}

void FriendLinkerHolder::return_freelink(FriendServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	//hash services
	int cid = link->get_frdhash();
	if (cid != -1)
	{
		//针对多个相同mailhash的mail服务切换时造成的
		boost::unordered_map<int, FriendServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			FriendServiceLinkFrom* p = fiter->second;
			if (p == link)
				hash_service_.erase(fiter);
		}
	}
	
	std::vector<FriendServiceLinkFrom*>::iterator fiter2 = std::find(friend_vector_.begin(), friend_vector_.end(), link);
	if (fiter2 != friend_vector_.end())
		friend_vector_.erase(fiter2);

	online_links_.erase(link->get_iid());
	free_links_.insert(link);
}

FriendServiceLinkFrom* FriendLinkerHolder::regist_onlinelink(FriendServiceLinkFrom* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_[link->get_iid()] = link;

	bool need_add = false;
	int cid = link->get_frdhash();
	if (cid != -1)
	{
		//针对多个相同mailhash的mail服务切换时造成的
		boost::unordered_map<int, FriendServiceLinkFrom*>::iterator fiter = hash_service_.find(cid);
		if (fiter != hash_service_.end())
		{
			FriendServiceLinkFrom* p = fiter->second;
			//iid大的表示后续启动的，替换前次启动的，每个mailhash服务只能存在一个
			if (link->get_iid() > p->get_iid())
			{
				hash_service_[cid] = link;
				need_add = true;
			}
		}
		else
		{
			hash_service_[cid] = link;
			need_add = true;
		}
	}

	if (need_add)
	{
		std::vector<FriendServiceLinkFrom*>::iterator fiter2 = std::find(friend_vector_.begin(), friend_vector_.end(), link);
		if (fiter2 == friend_vector_.end())
			friend_vector_.push_back(link);
	}

	return link;
}

void FriendLinkerHolder::send_mth_protocol(int frdhash, BasicProtocol* pro)
{
	ThreadLockWrapper guard(lock_);

	boost::unordered_map<int, FriendServiceLinkFrom*>::iterator fiter = hash_service_.find(frdhash);
	if (fiter == hash_service_.end())
	{
		delete pro;
		return;
	}

	FriendServiceLinkFrom* p = fiter->second;
	p->send_protocol(pro);
}

void FriendLinkerHolder::send_mth_protocol_circle(BasicProtocol* pro)
{
	ThreadLockWrapper guard(lock_);

	if (friend_vector_.size() == 0)
	{
		delete pro;
		return;
	}

	if (cur_friend_service_ >= friend_vector_.size())
		cur_friend_service_ = 0;

	FriendServiceLinkFrom* p = friend_vector_[cur_friend_service_];
	p->send_protocol(pro);

	++cur_friend_service_;
}