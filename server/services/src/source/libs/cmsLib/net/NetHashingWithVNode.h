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

#ifndef __NETHASHINGWITHVNODE_H__
#define __NETHASHINGWITHVNODE_H__

#include <cmsLib/core_type.h>
#include <map>
#include <list>
#include <cmsLib/hash/HashUtil.h>
#include <cmsLib/util/ShareUtil.h>

template<class T>
class NetHashingWithVNode
{
public:
	class VNodeWrap
	{
		friend class NetHashingWithVNode;
	public:
		VNodeWrap():node_(0), virtual_seq_(0){}

		void build_vnode(S_INT_64 key, int seq, T node) {
			this->key_ = key;
			this->virtual_seq_ = seq;
			this->node_ = node;

			std::string vkey = ShareUtil::str_format<64>("%lld#%d", key_, virtual_seq_);
			hash_key_ = HashUtil::fnv1_32_hash(vkey);
		}

		S_INT_32 get_hashkey() { return hash_key_; }

	protected:
		T			node_;		
		S_INT_64	key_;
		S_INT_32	hash_key_;

		S_INT_32	virtual_seq_;
	};
public:
	NetHashingWithVNode();

	void init_vnode(S_INT_32 v) { virtual_nodes_ = v; }

	void clear_nodes() {
		real_nodes_.clear();
	}

	void add_realnode(S_INT_64 key, T node);
	void build_nethashing();

	T get_netnode_byhash(S_INT_32 hash);
	T get_netnode_byval(S_INT_64 iid) {
		std::string sk = ShareUtil::str_format<32>("%lld", iid);
		S_INT_32 hash = HashUtil::fnv1_32_hash(sk);
		return get_netnode_byhash(hash);
	}

	//static void test();

private:
	std::map<S_INT_32, VNodeWrap>	net_nodes_;

	S_INT_32				virtual_nodes_;
	std::list<VNodeWrap>	real_nodes_;
};

template<class T>
NetHashingWithVNode<T>::NetHashingWithVNode():virtual_nodes_(1000)
{
}

template<class T>
void NetHashingWithVNode<T>::add_realnode(S_INT_64 key, T node)
{
	VNodeWrap vn;
	vn.key_ = key;
	vn.node_ = node;

	real_nodes_.push_back(vn);
}

template<class T>
void NetHashingWithVNode<T>::build_nethashing()
{
	net_nodes_.clear();

	for (typename std::list<VNodeWrap>::iterator iter = real_nodes_.begin(); iter != real_nodes_.end(); ++iter)
	{
		const VNodeWrap& v = (*iter);
		for (int ii = 0; ii < virtual_nodes_; ++ii)
		{
			VNodeWrap vn;
			vn.build_vnode(v.key_, ii, v.node_);

			net_nodes_[vn.get_hashkey()] = vn;
		}
	}
}

template<class T>
T NetHashingWithVNode<T>::get_netnode_byhash(S_INT_32 hash)
{
	//无任何内容
	if (net_nodes_.size() == 0)
		return 0;

	typename std::map<S_INT_32, VNodeWrap>::iterator iter = net_nodes_.upper_bound(hash);
	if (iter == net_nodes_.end())
		iter = net_nodes_.begin();

	return iter->second.node_;
}

/*
template<class T, int VNODE_NUM>
void NetHashingWithVNode<T, VNODE_NUM>::test()
{
	S_INT_64 iids[] = { 101,20};
	NetHashingWithVNode<S_INT_64, VNODE_NUM> vnodes;
	for (int ii = 0; ii < sizeof(iids) / sizeof(S_INT_64); ++ii)
		vnodes.add_realnode(iids[ii], &iids[ii]);
	vnodes.build_nethashing();

	std::map<S_INT_64, S_INT_32> hits;
	S_INT_64 s_max = 1000;
	for (S_INT_64 ii = 0; ii < s_max; ++ii)
	{
		S_INT_64* kid = vnodes.get_netnode_byval(ii);

		S_INT_64 k1 = *kid;
		S_INT_32 v1 = 0;
		std::map<S_INT_64, S_INT_32>::iterator fiter = hits.find(k1);
		if (fiter != hits.end())
			v1 = fiter->second;
		++v1;
		hits[k1] = v1;
	}

	std::list<double> phits;
	for (std::map<S_INT_64, S_INT_32>::iterator iter = hits.begin(); iter != hits.end(); ++iter)
	{
		S_INT_32 v1 = iter->second;
		double f = (v1*1.0) / s_max;
		phits.push_back(f);
	}

	return;
}
*/

#endif //__NETHASHINGWITHVNODE_H__
