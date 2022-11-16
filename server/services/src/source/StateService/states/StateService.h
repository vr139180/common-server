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

#ifndef __STATESERVICE_H__
#define __STATESERVICE_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>
#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/redis/RedisProtoBufThreadCache.h>
#include <cmsLib/lua/ScriptContext.h>

class StateService : public VirtualMainThread , public MessageProcess
{
	typedef VirtualMainThread base;
public:
	StateService();
	virtual ~StateService();

	void init_state();
	void reset_syscmd();

private:
	RedisClient					redis_;
	RedisProtoBufThreadCache	redisproto_cache_;

protected:
	void reset_state( void*);
	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}

public:
	void on_user_login_req(NetProtocol* pro, bool& autorelease);
	void on_user_relogin_req(NetProtocol* pro, bool& autorelease);
	void on_user_active_ntf(NetProtocol* pro, bool& autorelease);
};

#endif //__STATESERVICE_H__
