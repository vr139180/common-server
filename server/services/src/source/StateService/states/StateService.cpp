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

#include "states/StateService.h"

#include "config/StateConfig.h"
#include "StateServiceApp.h"

StateService::StateService():base()
{
}

StateService::~StateService()
{
}

void StateService::thread_worker()
{
	//bind to thread
	svrApp.get_redisclient_thread().reset(&redis_);
	svrApp.get_rpcache_thread().reset(&redisproto_cache_);

	int sleepstep = 0;
	while (1)
	{
		if (sleepstep >= loop_num_)
		{
			OSSystem::mOS->thread_yield();
			sleepstep = 0;
		}
		++sleepstep;

		if (will_quit_) break;

		CommandBase *pCmd = pop_one_cmd();
		std::unique_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loop_num_;
			continue;
		}

		pCmd->run();
	}

	svrApp.get_redisclient_thread().release();
	svrApp.get_rpcache_thread().release();
}

void StateService::init_state()
{
	this->InitNetMessage();

	//³õÊ¼»¯redisÉèÖÃ
	StateConfig *conf_ = svrApp.get_config();
	redis_.init_redis(conf_->redis_.ip_, conf_->redis_.port_, conf_->redis_.auth_,
		conf_->redis_.db_, conf_->redis_.socket_timeout_);
}

void StateService::reset_state( void*)
{

}

void StateService::reset_syscmd()
{
	SystemCommand<void>* cmd =
		new SystemCommand<void>(boost::bind(&StateService::reset_state, this, this));
	this->regist_syscmd(cmd);
}
