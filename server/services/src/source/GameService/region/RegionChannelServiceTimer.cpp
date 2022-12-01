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

#include "region/RegionChannelService.h"

#include <gameLib/global_const.h>

#include "GameServiceApp.h"

void RegionChannelService::regist_timer()
{
	channel_timer_.add_timer(1000, boost::BOOST_BIND(&RegionChannelService::on_userinout_check_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	channel_timer_.add_timer(1000*8, boost::BOOST_BIND(&RegionChannelService::on_userlive_check_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void RegionChannelService::on_userlive_check_timer(u64 tnow, int interval, u64 iid, bool& finish)
{

}

void RegionChannelService::on_userinout_check_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	region_map_->user_inout_tick();
}
