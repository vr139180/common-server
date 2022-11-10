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

#ifndef __IEUREKANODELINK_H__
#define __IEUREKANODELINK_H__

#include <cmsLib/core_type.h>

class IEurekaNodeLink
{
public:
	IEurekaNodeLink() {}

	virtual S_INT_64 get_iid() = 0;
	virtual S_INT_64 get_token() = 0;

	virtual void send_to_eureka(BasicProtocol* pro) = 0;
	virtual bool is_ready() = 0;

	virtual bool is_link_to() = 0;

	virtual void force_linkclose() = 0;

	virtual void heart_beat() = 0;
};

#endif //__IEUREKANODELINK_H__