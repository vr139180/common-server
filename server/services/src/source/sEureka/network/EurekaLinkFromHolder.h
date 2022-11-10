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

#ifndef __EUREKALINKFROMHOLDER_H__
#define __EUREKALINKFROMHOLDER_H__

#include <set>
#include <list>
#include <boost/unordered_map.hpp>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/core_type.h>

template<typename T>
class EurekaLinkFromHolder
{
	typedef boost::unordered_map<S_INT_64, T*> SERVICEMAP;
public:
	EurekaLinkFromHolder();
	~EurekaLinkFromHolder();
	void init_holder();
	virtual void uninit_holder();

	T* ask_free_link();
	T* get_eurekalink_byiid(S_INT_64 iid);
	virtual void return_freelink(T* link);
	virtual T* regist_onlinelink(T* link);

	template<class M>
	void broadcast(M* pro)
	{
		ThreadLockWrapper guard(lock_);

		typename boost::unordered_map<S_INT_64, T*>::iterator iter = online_links_.begin();
		for (; iter != online_links_.end(); ++iter)
		{
			M* msg = new M();
			msg->CopyFrom(*pro);

			T* link = iter->second;
			link->send_protocol(msg);
		}
	}

protected:
	std::list<T*>	all_service_link_;
	std::set<T*>	free_links_;
	SERVICEMAP		online_links_;

	ThreadLock	lock_;
};

template<typename T>
EurekaLinkFromHolder<T>::EurekaLinkFromHolder()
{
}

template<typename T>
EurekaLinkFromHolder<T>::~EurekaLinkFromHolder()
{
	uninit_holder();
}

template<typename T>
void EurekaLinkFromHolder<T>::init_holder()
{
	ThreadLockWrapper guard(lock_);
}

template<typename T>
void EurekaLinkFromHolder<T>::uninit_holder()
{
	ThreadLockWrapper guard(lock_);

	online_links_.clear();
	free_links_.clear();

	typename std::list<T*>::iterator iter = all_service_link_.begin();
	for (; iter != all_service_link_.end(); ++iter)
	{
		delete (*iter);
	}
	all_service_link_.clear();
}

template<typename T>
T* EurekaLinkFromHolder<T>::ask_free_link()
{
	ThreadLockWrapper guard(lock_);

	T* ret = 0;
	if (free_links_.size() == 0)
	{
		ret = new T();
		all_service_link_.push_back(ret);
	}
	else
	{
		typename std::set<T*>::iterator iter = free_links_.begin();
		ret = (*iter);
		free_links_.erase(iter);
	}

	return ret;
}

template<typename T>
void EurekaLinkFromHolder<T>::return_freelink(T* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_.erase(link->get_iid());
	free_links_.insert(link);
}

template<typename T>
T* EurekaLinkFromHolder<T>::regist_onlinelink(T* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_[link->get_iid()] = link;
	return link;
}

template<typename T>
T* EurekaLinkFromHolder<T>::get_eurekalink_byiid(S_INT_64 iid)
{
	ThreadLockWrapper guard(lock_);

	typename boost::unordered_map<S_INT_64, T*>::iterator fiter = online_links_.find(iid);
	if (fiter == online_links_.end())
		return 0;
	return fiter->second;
}

#endif //__EUREKALINKFROMHOLDER_H__