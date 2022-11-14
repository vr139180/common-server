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

#ifndef __EUREKALOSTMAINTANCE_H__
#define __EUREKALOSTMAINTANCE_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/ThreadLock.h>
#include <set>

#include <gameLib/eureka/EurekaNodeInfo.h>
#include <gameLib/global_const.h>

class EurekaLostMaintance
{
public:
	EurekaLostMaintance(S_INT_64 nodeiid);
	virtual ~EurekaLostMaintance();

	S_INT_64 get_nodeiid() { return node_iid_; }
	bool is_timeout();
	bool is_max_retry() {return try_nums_ >= EUREKA_LOST_RETRY;}
	void somebody_tellme_lost(S_INT_64 fromiid);
	S_INT_32 tellme_somebody_num() { return (S_INT_32)tellme_lost_nodes_.size(); }

	//连接重试
	void try_reconnect();
	void rebind() { rebinded_ = true; }
	bool is_rebinded() { return rebinded_; }

protected:
	//重试次数
	S_INT_32	try_nums_;
	std::set<S_INT_64>	tellme_lost_nodes_;
	//重新绑定成功
	bool		rebinded_;

private:
	S_INT_64	node_iid_;
	S_INT_64	lost_time_;
};

#endif //__EUREKALOSTMAINTANCE_H__