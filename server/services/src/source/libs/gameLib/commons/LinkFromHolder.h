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

#ifndef __LINKFROMHOLDER_H__
#define __LINKFROMHOLDER_H__

#include <set>
#include <list>
#include <boost/unordered_map.hpp>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/core_type.h>

template<typename T>
class LinkFromHolder
{
	typedef boost::unordered_map<S_INT_64, T*> SERVICEMAP;
public:
	LinkFromHolder();
	virtual ~LinkFromHolder();
	void init_holder();
	virtual void uninit_holder();

	T* ask_free_link();
	T* get_servicelink_byiid(S_INT_64 iid);
	virtual void return_freelink(T* link);
	virtual T* regist_onlinelink(T* link);
	T* get_servicelink_random();

	//pro由调用者管理内存
	void broadcast(BasicProtocol* pro)
	{
		ThreadLockWrapper guard(lock_);

		typename boost::unordered_map<S_INT_64, T*>::iterator iter = online_links_.begin();
		for (; iter != online_links_.end(); ++iter)
		{
			BasicProtocol* msg = pro->New();
			msg->CopyFrom(*pro);

			T* link = iter->second;
			link->send_netprotocol(msg);
		}
	}

	void broadcast(NetProtocol* pro)
	{
		ThreadLockWrapper guard(lock_);

		typename boost::unordered_map<S_INT_64, T*>::iterator iter = online_links_.begin();
		for (; iter != online_links_.end(); ++iter)
		{
			NetProtocol* np = pro->clone();

			T* link = iter->second;
			link->send_protocol( np);
		}
	}

protected:
	std::list<T*>	all_service_link_;
	std::set<T*>	free_links_;
	SERVICEMAP		online_links_;
	S_INT_32		random_online_;

	ThreadLock	lock_;
};

template<typename T>
LinkFromHolder<T>::LinkFromHolder()
{
}

template<typename T>
LinkFromHolder<T>::~LinkFromHolder()
{
	uninit_holder();
}

template<typename T>
void LinkFromHolder<T>::init_holder()
{
	ThreadLockWrapper guard(lock_);

	random_online_ = 0;
}

template<typename T>
void LinkFromHolder<T>::uninit_holder()
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
T* LinkFromHolder<T>::ask_free_link()
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
void LinkFromHolder<T>::return_freelink(T* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_.erase(link->get_iid());
	free_links_.insert(link);
}

template<typename T>
T* LinkFromHolder<T>::regist_onlinelink(T* link)
{
	ThreadLockWrapper guard(lock_);

	online_links_[link->get_iid()] = link;
	return link;
}

template<typename T>
T* LinkFromHolder<T>::get_servicelink_byiid(S_INT_64 iid)
{
	ThreadLockWrapper guard(lock_);

	typename SERVICEMAP::iterator fiter = online_links_.find(iid);
	if (fiter == online_links_.end())
		return 0;
	return fiter->second;
}

template<typename T>
T* LinkFromHolder<T>::get_servicelink_random()
{
	if (online_links_.size() == 0)
		return 0;

	ThreadLockWrapper guard(lock_);
	++random_online_;
	if (random_online_ >= online_links_.size())
		random_online_ = 0;

	typename SERVICEMAP::iterator iter = online_links_.begin();
	for (int ii = 0; ii < random_online_; ++ii)
		++iter;
	return iter->second;
}

#endif //__LINKFROMHOLDER_H__