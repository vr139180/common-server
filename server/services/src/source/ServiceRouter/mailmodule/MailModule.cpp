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
