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

#ifndef __LOBBYUSERCONTAINER_H__
#define __LOBBYUSERCONTAINER_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/base/OSSystem.h>

class LobbyService;

template<typename T>
class LobbyUserContainer
{
protected:
	class DoubleLinkNode
	{
	public:
		DoubleLinkNode(){
			reset();
		}

		void reset() {
			data_ = 0;
			pre_ = 0;
			next_ = 0;
			last_time_ = 0;
		}

		void reuse(T* d) {
			data_ = d;
			last_time_ = OSSystem::mOS->GetTimestamp();
		}

		bool need_move() {
			S_INT_64 tnow = OSSystem::mOS->GetTimestamp();
			if ((last_time_ + 10 * 1000) < tnow)
			{
				last_time_ = tnow;
				return true;
			}
			else
				return false;
		}

		T* data() {
			return data_;
		}

		void add_head(DoubleLinkNode* head) {
			this->pre_ = 0;
			this->next_ = head;
			head->pre_ = this;
		}

		void move_to_head(DoubleLinkNode* head, DoubleLinkNode** tail) {
			if (this == head)
				return;

			if (this == *tail)
				*tail = this->pre_;

			DoubleLinkNode* next = this->next_;
			if (pre_ != 0)
				pre_->next_ = next;
			if (next != 0)
				next->pre_ = pre_;

			add_head(head);
		}

		void free_node(DoubleLinkNode**head, DoubleLinkNode** tail) {
			DoubleLinkNode* ph = *head;
			DoubleLinkNode* pt = *tail;

			if (this == ph)
			{
				if (ph->next_)
					ph->next_->pre_ = 0;
				*head = this->next_;
				this->next_ = 0;
			}
			if (this == pt)
			{
				if (pt->pre_)
					pt->pre_->next_ = 0;
				*tail = pt->pre_;
				this->pre_ = 0;
			}
			
			ph = this->pre_;
			pt = this->next_;
			if (ph != 0)
			{
				ph->next_ = pt;
				this->pre_ = 0;
			}
			if (pt != 0)
			{
				pt->pre_ = ph;
				this->next_ = 0;
			}

			this->last_time_ = 0;
			this->data_ = 0;
		}

	private:
		DoubleLinkNode* pre_;
		DoubleLinkNode* next_;

		S_INT_64	last_time_;

		T* data_;
	};

	typedef boost::unordered_map<S_INT_64, DoubleLinkNode*>	USER_MAP_DATA;

public:

	LobbyUserContainer();
	~LobbyUserContainer();

	//初始化可用的对象
	void init_cap( LobbyService* p, int minsize = 1000);

	//获取用户，如果没有加入
	T* get_lobbyuser(S_INT_64 userid);
	void free_lobbyuser(S_INT_64 userid);

	void get_all_users(std::list<T*>& dats);

protected:
	void release();

	DoubleLinkNode* get_free_node() {
		DoubleLinkNode* pnod = 0;
		if (free_nodes_.size() > 0)
		{
			pnod = *(free_nodes_.begin());
			free_nodes_.pop_front();
		}
		else
		{
			pnod = new DoubleLinkNode();
		}

		T* pdat = 0;
		if (free_data_.size() > 0)
		{
			pdat = *(free_data_.begin());
			free_data_.pop_front();
		}
		else
		{
			pdat = new T();
			pdat->set_context(owner_);

			all_datas_.push_back(pdat);
		}

		pnod->reuse(pdat);

		return pnod;
	}

private:
	//userid -> data
	USER_MAP_DATA	user_map_;
	DoubleLinkNode	*head_;
	DoubleLinkNode	*tail_;

	//free node
	std::list<DoubleLinkNode*>	free_nodes_;

	//free data
	std::list<T*>	free_data_;
	//data save
	std::list<T*>	all_datas_;

	LobbyService*	owner_;
};

template<typename T>
LobbyUserContainer<T>::LobbyUserContainer():head_(0), tail_(0)
{
}

template<typename T>
LobbyUserContainer<T>::~LobbyUserContainer()
{
	release();
}

template<typename T>
void LobbyUserContainer<T>::init_cap(LobbyService* p, int minsize)
{
	owner_ = p;
	for (int ii = 0; ii < minsize; ++ii)
	{
		T* dat = new T();
		dat->set_context(owner_);

		all_datas_.push_back(dat);
		free_data_.push_back(dat);

		DoubleLinkNode* pnod = new DoubleLinkNode();
		free_nodes_.push_back(pnod);
	}
}

template<typename T>
void LobbyUserContainer<T>::get_all_users(std::list<T*>& dats)
{
	for (typename USER_MAP_DATA::iterator iter = user_map_.begin(); iter != user_map_.end(); ++iter)
	{
		DoubleLinkNode* pnod = iter->second;
		dats.push_back(pnod->data());
	}
}

template<typename T>
T* LobbyUserContainer<T>::get_lobbyuser(S_INT_64 userid)
{
	DoubleLinkNode* pnod = 0;
	typename USER_MAP_DATA::iterator fiter = user_map_.find(userid);
	if (fiter != user_map_.end())
		pnod = fiter->second;

	if (pnod == 0)
	{
		pnod = get_free_node();
		user_map_[userid] = pnod;
		//add to double link
		if (head_ == 0)
		{
			head_ = pnod;
			tail_ = head_;
		}
		else
		{
			pnod->add_head(head_);
		}
	}

	if (pnod->need_move())
	{
		pnod->move_to_head(head_, &tail_);
	}

	return pnod->data();
}

template<typename T>
void LobbyUserContainer<T>::free_lobbyuser(S_INT_64 userid)
{
	DoubleLinkNode* pnod = 0;
	typename USER_MAP_DATA::iterator fiter = user_map_.find(userid);
	if (fiter == user_map_.end())
		return;
		
	pnod = fiter->second;
	T* pdata = pnod->data();
	
	pnod->free_node( &head_, &tail_);
	
	free_nodes_.push_back(pnod);
	pdata->rest_user();
	free_data_.push_back(pdata);
}

template<typename T>
void LobbyUserContainer<T>::release()
{
	free_data_.clear();
	head_ = 0;
	tail_ = 0;

	for (typename USER_MAP_DATA::iterator iter = user_map_.begin(); iter != user_map_.end(); ++iter)
		delete iter->second;
	user_map_.clear();

	for (typename std::list<DoubleLinkNode*>::iterator iter = free_nodes_.begin(); iter != free_nodes_.end(); ++iter)
		delete (*iter);
	free_nodes_.clear();

	for (typename std::list<T*>::iterator iter = all_datas_.begin(); iter != all_datas_.end(); ++iter)
		delete (*iter);
	all_datas_.clear();
}

#endif //__LOBBYUSERCONTAINER_H__