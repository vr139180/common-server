#ifndef __LINKTOHOLDER_H__
#define __LINKTOHOLDER_H__

#include <list>
#include <vector>
#include <set>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

template<typename T>
class LinkToHolder
{
public:
	LinkToHolder();
	virtual ~LinkToHolder();

	void free_all();

	void connect_to();

	//和eureka同步服务
	void sync_eureka_services(std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);
	void send_mth_protocol(BasicProtocol* pro);

	template<class M>
	void broadcast(M* pro)
	{
		ThreadLockWrapper guard(lock_);

		for( typename std::vector<T*>::iterator iter = online_links_.begin(); iter != online_links_.end(); ++iter)
		{
			M* msg = new M();
			msg->CopyFrom(*pro);

			T* link = (*iter);
			link->send_protocol(msg);
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
LinkToHolder<T>::LinkToHolder():cur_online_link_(0)
{
}

template<typename T>
LinkToHolder<T>::~LinkToHolder()
{
	release();
}

template<typename T>
void LinkToHolder<T>::release()
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
void LinkToHolder<T>::free_all()
{
	release();
}

template<typename T>
bool LinkToHolder<T>::is_service_exist(S_INT_64 sid)
{
	std::set<S_INT_64>::iterator fiter = services_iid.find(sid);
	return fiter != services_iid.end();
}

template<typename T>
bool LinkToHolder<T>::is_service_process(S_INT_64 sid)
{
	std::set<S_INT_64>::iterator fiter = service_process_iid.find(sid);
	return fiter != service_process_iid.end();
}

template<typename T>
void LinkToHolder<T>::connect_to()
{
	if (wait_links_.size() == 0)
		return;

	for( typename std::set<T*>::iterator iter = wait_links_.begin(); iter != wait_links_.end(); ++iter)
	{
		T* pnode = (*iter);
		pnode->connect();

		auth_links_.insert(pnode);
		service_process_iid.insert(pnode->get_iid());
	}
	wait_links_.clear();
}

template<typename T>
void LinkToHolder<T>::send_mth_protocol(BasicProtocol* pro)
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

	online_links_[cur_online_link_]->send_protocol(pro);
}

template<typename T>
void LinkToHolder<T>::on_linkto_disconnected(T* plink)
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

	plink->force_linkclose();
}

template<typename T>
void LinkToHolder<T>::on_linkto_regist_result( T* plink)
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
void LinkToHolder<T>::sync_eureka_services(std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
	ThreadLockWrapper guard(lock_);

	for (std::list<ServiceNodeInfo*>::iterator iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		ServiceNodeInfo* pnode = (*iter);

		if (is_service_exist(pnode->iid))
			continue;

		//增加新注册的
		services_iid.insert(pnode->iid);

		//判断是否需要加入新的链接
		if (is_service_process(pnode->iid) == false)
		{
			T* newnode = 0;
			if (free_links_.size() <= 0)
			{
				newnode = new T( pnode->clone());
				links_cache_.push_back(newnode);
			}
			else
			{
				typename std::set<T*>::iterator fiter = free_links_.begin();
				newnode = (*fiter);
				free_links_.erase(fiter);

				newnode->reset(pnode->clone());
			}

			//加入待链接队列
			wait_links_.insert(newnode);
			service_process_iid.insert(newnode->get_iid());
		}
	}

	//移除已注册的
	for (std::list<S_INT_64>::iterator iter = deliids.begin(); iter != deliids.end(); ++iter)
	{
		S_INT_64 sid = (*iter);
		services_iid.erase(sid);
	}
}

#endif //__LINKTOHOLDER_H__