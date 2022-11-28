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

#ifndef __REGIONCHANNELSERVICE_H__
#define __REGIONCHANNELSERVICE_H__

#include <boost/scoped_ptr.hpp>
#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>
#include "region/GamePlayer.h"
#include "region/RegionPlayerContainer.h"
#include "region/IRegionMap.h"

class RegionChannelService : public VirtualMainThread, public MessageProcess
{
	typedef VirtualMainThread base;
public:
	RegionChannelService();
	virtual ~RegionChannelService();

	void init_channel( S_INT_32 cid);
	void build_gameid(S_INT_64 serviceid);

private:
	RegionPlayerContainer<GamePlayer>	channel_users_;

	boost::scoped_ptr<IRegionMap>	region_map_;

	//±àÂëºóµÄgameid
	S_INT_64	gameid_;
	S_INT_32	channel_index_;

protected:
	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}
	virtual void NetProcessMessage(NetProtocol* message, bool& autorelease);

protected:
};

#endif //__REGIONCHANNELSERVICE_H__
