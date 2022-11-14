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

#include "chatmodule/ChatModule.h"

#include <cmsLib/util/Random.h>

#include "ServiceRouterApp.h"

//自定义custom channel的channel根据serviceid+sequence生成，由serviceid的唯一性保证序列号唯一
//20bit servicei 43bit
#define CUSTOMCHANNEL_MASK_ZERO_H	0b0000000000000000000001111111111111111111111111111111111111111111
#define CUSTOMCHANNEL_MASK_ZERO_L	0b1111111111111111111110000000000000000000000000000000000000000000

ChatModule& ChatModule::instance()
{
	static ChatModule s_instance;
	return s_instance;
}

ChatModule::ChatModule() :channel_seed_(0)
{
	iids_ = 0;
}

void ChatModule::init_chatmodule(S_INT_64 myiid)
{
	//init plot
	this->chash_plot_ = svrApp.get_config()->chathash_plot_;

	this->channel_seed_ = myiid;
	this->iids_ = (S_INT_64)CMS_RandMinMax(10, 1000);
}

S_INT_64 ChatModule::new_custom_channelid()
{
	this->iids_++;
	return (((channel_seed_ << 43)&CUSTOMCHANNEL_MASK_ZERO_L) | iids_);
}

int ChatModule::channelid_to_chathash(int type, S_INT_64 channelid)
{
	return (channelid % chash_plot_.chatmax);
}
