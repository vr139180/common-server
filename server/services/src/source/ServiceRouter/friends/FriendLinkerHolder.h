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

#ifndef __FRIENDLINKERHOLDER_H__
#define __FRIENDLINKERHOLDER_H__

#include <gameLib/commons/LinkFromHolder.h>
#include "network/FriendServiceLinkFrom.h"

class FriendLinkerHolder : public LinkFromHolder<FriendServiceLinkFrom>
{
	typedef LinkFromHolder<FriendServiceLinkFrom> base;
public:
	FriendLinkerHolder();

	virtual void uninit_holder();

	virtual void return_freelink(FriendServiceLinkFrom* link);
	virtual FriendServiceLinkFrom* regist_onlinelink(FriendServiceLinkFrom* link);

	void send_mth_protocol(int mailhash, BasicProtocol* pro);
	void send_mth_protocol_circle(BasicProtocol* pro);

protected:
	boost::unordered_map<int, FriendServiceLinkFrom*>	hash_service_;
	std::vector<FriendServiceLinkFrom*>					friend_vector_;

	int		cur_friend_service_;
};

#endif //__FRIENDLINKERHOLDER_H__
