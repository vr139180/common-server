#include "friends/FriendModule.h"

#include <cmsLib/util/Random.h>

#include "RouterServiceApp.h"

FriendModule::FriendModule()
{
}

FriendModule& FriendModule::instance()
{
	static FriendModule s_instance;
	return s_instance;
}

void FriendModule::init_friendmodule(S_INT_64 myiid)
{
	//init plot
	this->fhash_plot_ = svrApp.get_config()->friendhash_plot_;
}

int FriendModule::user_to_frdhash(S_INT_64 useriid)
{
	return (useriid % fhash_plot_.frdmax);
}
