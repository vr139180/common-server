#include "EurekaService.h"

#include "sEurekaApp.h"

EurekaService::EurekaService():
	will_quit_(false),
	loop_num_(1)
{

}

EurekaService::~EurekaService()
{

}

void EurekaService::init(EurekaConfig* conf)
{
	this->loop_num_ = conf->loopnum_;

	this->redis_.init_redis(conf->redis_.ip_, conf->redis_.port_, conf->redis_.auth_,
		conf->redis_.db_, conf->redis_.socket_timeout_);
}

void EurekaService::start()
{
	stop();

	will_quit_ = false;

	boost::function0<void> f = boost::bind(&EurekaService::thread_worker, this);
	thread_ = boost::shared_ptr<boost::thread>(new boost::thread(f));
}

void EurekaService::stop()
{
	will_quit_ = true;

	if (thread_.get() == 0)
		return;

	thread_->join();
	thread_.reset<boost::thread>(0);
}

void EurekaService::thread_worker()
{
	int sleepstep = 0;
	u64 last_check = 0;
	svrApp.get_redisclient_thread().reset( &(this->redis_));

	while (1)
	{
		if (sleepstep >= loop_num_)
		{
			u64 tnow = OSSystem::mOS->GetTicksRealtime();

			OSSystem::mOS->thread_yield();
			sleepstep = 0;
		}
		++sleepstep;

		if (will_quit_) break;

		//服务线程只处理 netcommand, usercommand.不处理systemcommand
		CommandBase *pCmd = svrApp.pop_net_cmd();
		std::unique_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loop_num_;
			continue;
		}
		pCmd->run();
	}

	svrApp.get_redisclient_thread().release();
}
