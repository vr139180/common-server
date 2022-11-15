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

#include "cluster/EurekaLostMaintance.h"

#include <cmsLib/base/OSSystem.h>
#include <gameLib/global_const.h>
#include <gameLib/LogExt.h>

#include "cluster/EurekaClusterCtrl.h"

#include "sEurekaApp.h"

EurekaLostMaintance::EurekaLostMaintance(S_INT_64 nodeiid, S_INT_64 lostmid):node_iid_(nodeiid), lost_master_iid_( lostmid)
{
	lost_time_ = (S_INT_64)OSSystem::mOS->GetTimestamp();
	try_nums_ = 0;
	rebinded_ = false;
}

EurekaLostMaintance::~EurekaLostMaintance()
{
}

bool EurekaLostMaintance::is_timeout()
{
	S_INT_64 tnow = (S_INT_64)OSSystem::mOS->GetTimestamp();
	return (lost_time_ + EUREKA_LOST_TIMEOUT) < tnow;
}

void EurekaLostMaintance::try_reconnect()
{
	if (svrApp.get_eurekactrl()->get_myself().iid <= node_iid_)
		return;

	//ÖØÊÔ3´Î
	if (try_nums_ >= EUREKA_LOST_RETRY)
		return;

	if (svrApp.get_eurekactrl()->force_reconnect(node_iid_))
	{
		++try_nums_;
		logInfo(out_runtime, "lost eureka node:%d, retry:%d connect to it.", node_iid_, try_nums_);
	}
}
