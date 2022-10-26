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
