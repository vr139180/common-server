#include "mailmodule/MailModule.h"

#include <cmsLib/util/Random.h>

#include "ServiceRouterApp.h"

MailModule::MailModule()
{
}

MailModule& MailModule::instance()
{
	static MailModule s_instance;
	return s_instance;
}

void MailModule::init_mailmodule(S_INT_64 myiid)
{
	//init plot
	this->mhash_plot_ = svrApp.get_config()->mailhash_plot_;
}

int MailModule::receiver_to_mailhash(S_INT_64 receiver)
{
	return (receiver % mhash_plot_.mailmax);
}
