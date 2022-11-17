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

#ifndef __EUREKALINKTOHOLDER_H__
#define __EUREKALINKTOHOLDER_H__

#include <list>
#include <vector>
#include <set>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/eureka/EurekaNodeInfo.h>
#include <gameLib/LogExt.h>

template<typename T>
class EurekaLinkToHolder
{
public:
	EurekaLinkToHolder();
	virtual ~EurekaLinkToHolder();

	void free_all();

	void connect_to();

	bool connect_to(S_INT_64 iid);

	T* get_eurekalink_byiid(S_INT_64 svriid);

	S_INT_32 get_authed_size() {
		return (S_INT_32)auth_links_.size();
	}

	//和eureka同步服务
	void add_linkto_node(EurekaNodeInfo* node);
	void remove_linkto_node(S_INT_64 nodeiid);

	void send_mth_protocol( NetProtocol* pro);

	void broadcast(BasicProtocol* pro, S_INT_64 ignore = 0)
	{
		ThreadLockWrapper guard(lock_);

		for( typename std::vector<T*>::iterator iter = online_links_.begin(); iter != online_links_.end(); ++iter)
		{
			T* link = (*iter);
			if (ignore != 0 && link->get_iid() == ignore)
				continue;

			logDebug(out_runtime, "---linkto broadcast msg:%s to eureka:%d", pro->GetTypeName().c_str(), link->get_iid());

			BasicProtocol* msg = pro->New();
			msg->CopyFrom(*pro);

			link->send_to_eureka(msg);
		}
	}

public:
	void on_linkto_disconnected( T* plink);
	void on_linkto_regist_result( T* plink);

protected:
	void release();

	bool is_service_exist(S_INT_64 sid);
	bool is_service_process(S_INT_64 sid);

protected:
	//保存对象，运行过程中只加不删
	std::list<T*>	links_cache_;
	//引用对象
	std::vector<T*>	online_links_;
	int cur_online_link_;

	std::set<T*>	auth_links_;
	std::set<T*>	wait_links_;
	//使用中的服务,处理因为eureka cluster的数据时效性问题导致的 服务订阅数据不同步
	std::set<S_INT_64>	service_process_iid;

	std::set<T*>	free_links_;

	//eureka注册的服务列表
	std::set<S_INT_64>	services_iid;

private:
	ThreadLock	lock_;
};

template<typename T>
EurekaLinkToHolder<T>::EurekaLinkToHolder():cur_online_link_(0)
{
}

template<typename T>
EurekaLinkToHolder<T>::~EurekaLinkToHolder()
{
	release();
}

template<typename T>
void EurekaLinkToHolder<T>::release()
{
	services_iid.clear();
	service_process_iid.clear();

	online_links_.clear();
	auth_links_.clear();
	wait_links_.clear();
	free_links_.clear();

	for( typename std::list<T*>::iterator iter = links_cache_.begin(); iter != links_cache_.end(); ++iter)
	{
		delete (*iter);
	}
	links_cache_.clear();
}

template<typename T>
void EurekaLinkToHolder<T>::free_all()
{
	release();
}

template<typename T>
bool EurekaLinkToHolder<T>::is_service_exist(S_INT_64 sid)
{
	std::set<S_INT_64>::iterator fiter = services_iid.find(sid);
	return fiter != services_iid.end();
}

template<typename T>
T* EurekaLinkToHolder<T>::get_eurekalink_byiid(S_INT_64 svriid)
{
	for (int ind = 0; ind < online_links_.size(); ++ind)
	{
		T* plink = online_links_[ind];
		if (plink->get_iid() == svriid)
			return plink;
	}

	return 0;
}

template<typename T>
bool EurekaLinkToHolder<T>::is_service_process(S_INT_64 sid)
{
	std::set<S_INT_64>::iterator fiter = service_process_iid.find(sid);
	return fiter != service_process_iid.end();
}

template<typename T>
void EurekaLinkToHolder<T>::connect_to()
{
	if (wait_links_.size() == 0)
		return;

	for( typename std::set<T*>::iterator iter = wait_links_.begin(); iter != wait_links_.end(); ++iter)
	{
		T* pnode = (*iter);

		if (!is_service_exist(pnode->get_iid()))
		{
			free_links_.insert(pnode);
			service_process_iid.erase(pnode->get_iid());
			continue;
		}

		pnode->connect();

		auth_links_.insert(pnode);
		service_process_iid.insert(pnode->get_iid());
	}
	wait_links_.clear();
}

template<typename T>
bool EurekaLinkToHolder<T>::connect_to(S_INT_64 iid)
{
	if (wait_links_.size() == 0)
		return false;

	for (typename std::set<T*>::iterator iter = wait_links_.begin(); iter != wait_links_.end(); ++iter)
	{
		T* pnode = (*iter);
		if (pnode->get_iid() != iid)
			continue;

		if (!is_service_exist(pnode->get_iid()))
		{
			free_links_.insert(pnode);
			service_process_iid.erase(pnode->get_iid());
			continue;
		}

		pnode->connect();

		auth_links_.insert(pnode);
		service_process_iid.insert(pnode->get_iid());
		wait_links_.erase(iter);

		return true;
	}
	
	return false;
}

template<typename T>
void EurekaLinkToHolder<T>::send_mth_protocol( NetProtocol* pro)
{
	size_t num = online_links_.size();
	if (num == 0)
	{
		delete pro;
		return;
	}

	ThreadLockWrapper guard(lock_);

	num = online_links_.size();
	if (num == 0)
	{
		delete pro;
		return;
	}

	++cur_online_link_;
	if (cur_online_link_ >= num)
		cur_online_link_ = 0;
	else if (cur_online_link_ < 0)
		cur_online_link_ = 0;

	online_links_[cur_online_link_]->send_to_eureka(to, pro);
}

template<typename T>
void EurekaLinkToHolder<T>::on_linkto_disconnected(T* plink)
{
	{
		ThreadLockWrapper guard(lock_);

		auth_links_.erase(plink);
		wait_links_.erase(plink);

		service_process_iid.erase(plink->get_iid());

		{
			typename std::vector<T*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
			if (fiter != online_links_.end())
				online_links_.erase(fiter);
		}

		if (is_service_exist(plink->get_iid()))
		{
			wait_links_.insert(plink);
			service_process_iid.insert(plink->get_iid());
		}
		else
		{
			free_links_.insert(plink);
		}
	}
	
	plink->force_linkclose();
}

template<typename T>
void EurekaLinkToHolder<T>::on_linkto_regist_result( T* plink)
{
	ThreadLockWrapper guard(lock_);

	auth_links_.erase(plink);
	wait_links_.erase(plink);

	service_process_iid.insert(plink->get_iid());

	typename std::vector<T*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
	if (fiter == online_links_.end())
		online_links_.push_back(plink);
}

template<typename T>
void EurekaLinkToHolder<T>::add_linkto_node(EurekaNodeInfo* pnode)
{
	ThreadLockWrapper guard(lock_);

	if (is_service_exist(pnode->iid))
		return;

	//增加新注册的
	services_iid.insert(pnode->iid);

	//判断是否需要加入新的链接
	if (is_service_process(pnode->iid) == false)
	{
		T* newnode = 0;
		if (free_links_.size() <= 0)
		{
			newnode = new T(*pnode);
			links_cache_.push_back(newnode);
		}
		else
		{
			typename std::set<T*>::iterator fiter = free_links_.begin();
			newnode = (*fiter);
			free_links_.erase(fiter);

			newnode->reset(*pnode);
		}

		//加入待链接队列
		wait_links_.insert(newnode);
		service_process_iid.insert(newnode->get_iid());
	}
}

template<typename T>
void EurekaLinkToHolder<T>::remove_linkto_node(S_INT_64 nodeiid)
{
	ThreadLockWrapper guard(lock_);

	services_iid.erase(nodeiid);
}

#endif //__EUREKALINKTOHOLDER_H__