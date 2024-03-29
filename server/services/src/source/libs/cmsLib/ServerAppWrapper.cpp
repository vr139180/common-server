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

#include <cmsLib/ServerAppWrapper.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/Log.h>
#include <cmsLib/prolib/ProtocolFactory.h>

ServerAppBase* ServerAppWrapper::app_ =0;

ServerAppWrapper::ServerAppWrapper( ServerAppBase* app)
{
	app_ =app;
}

void ServerAppWrapper::run()
{
	if( !OSSystem::init())
		return;

	OSSystem::mOS->UpdateNowTick();

    std::string logpro = OSSystem::work_path2_;
    logpro.append("/log_settings.ini");
	//init log system
	logInit( logpro.c_str());

	// Initialize the signal handlers
	signal( SIGINT, ServerAppWrapper::signal_handler);
	signal( SIGTERM, ServerAppWrapper::signal_handler);
#ifdef EW_PLATFORM_WINDOWS
	signal( SIGBREAK, ServerAppWrapper::signal_handler);
#else
	signal(SIGUSR1, ServerAppWrapper::signal_handler);
	signal(SIGUSR2, ServerAppWrapper::signal_handler);
#endif

	logInfo( out_runtime, "System OS: %s\n", OSSystem::mOSType.c_str());
	logInfo( out_runtime, "Process Id: %d\n", OSSystem::mOS->GetProcessId());

	OSSystem::mOS->UpdateNowTick();

	if( init_app())
		app_->main_loop();

	OSSystem::mOS->UpdateNowTick();

	uninit_app();

	logShutdown;

	//uninit system
	OSSystem::uninit();
}

bool ServerAppWrapper::init_app()
{
	if( !app_->load_config())
        return false;

	if( !app_->pre_init())
        return false;

	//init protocol factory
	ProtocolFactory::instance()->init_factory();

	if( !app_->init_network())
        return false;

    if( !app_->init_database())
        return false;

	app_->register_timer();

	if( !app_->init_finish())
		return false;

	return true;
}

void ServerAppWrapper::uninit_app()
{
	app_->uninit_database();

	app_->uninit_network();

	ProtocolFactory::instance()->uinit();

	app_->uninit();
}

void ServerAppWrapper::signal_handler( s32 sig)
{
	if ( sig == SIGINT || sig == SIGTERM 
#ifdef EW_PLATFORM_WINDOWS
		|| sig == SIGBREAK
#else
		|| sig == SIGUSR1 || sig == SIGUSR2
#endif
		)
	{
		logInfo(out_runtime, "signal handler:%d, exit service....", sig);

		if( app_)
			app_->quit_app();
	}
}
