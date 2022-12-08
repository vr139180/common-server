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

#ifndef __CONSISTENTHASHCHECKER_H__
#define __CONSISTENTHASHCHECKER_H__

#include <list>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/core_type.h>
#include <cmsLib/net/NetHashingWithVNode.h>

//一致性hash检查
class ConsistentHashChecker : public NetHashingWithVNode<S_INT_64>
{
	typedef NetHashingWithVNode<S_INT_64> base;
private:
	ConsistentHashChecker() :base(), myid_(0) {}
	virtual ~ConsistentHashChecker() {}

public:
	static ConsistentHashChecker& instance();

	void init_checker(S_INT_32 vnode);
	void init_myid(S_INT_64 iid);

	void sync_balance_services(std::list<S_INT_64>& iids);

	//是否需要重定位cluster
	bool is_need_recircle_cluster(S_INT_64 kkey);

private:
	S_INT_64	myid_;
	ThreadLock	lock_;
};

inline
void ConsistentHashChecker::init_checker(S_INT_32 vnode)
{
	ThreadLockWrapper guard(lock_);

	base::init_vnode(vnode);
}

inline
void ConsistentHashChecker::init_myid(S_INT_64 iid)
{
	this->myid_ = iid;
}

inline
void ConsistentHashChecker::sync_balance_services(std::list<S_INT_64>& iids)
{
	ThreadLockWrapper guard(lock_);

	clear_nodes();

	for (std::list<S_INT_64>::iterator iter = iids.begin(); iter != iids.end(); ++iter)
	{
		S_INT_64 iid = (*iter);
		add_realnode(iid, iid);
	}

	build_nethashing();
}

inline
bool ConsistentHashChecker::is_need_recircle_cluster(S_INT_64 kkey)
{
	ThreadLockWrapper guard(lock_);

	S_INT_64 tocluster = this->get_netnode_byval(kkey);
	return tocluster != myid_;
}

#endif //__CONSISTENTHASHCHECKER_H__