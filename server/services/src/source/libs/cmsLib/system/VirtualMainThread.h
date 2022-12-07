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

#ifndef __VIRTUALMAINTHREAD_H__
#define __VIRTUALMAINTHREAD_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/system/CommandBase.h>
#include <cmsLib/ThreadLock.h>

#include <boost/thread.hpp>

class VirtualMainThread
{
public:
	typedef std::list<CommandBase* >			COMMAND_LIST;
	typedef std::list<CommandBase* >::iterator	COMMAND_LIST_ITER;

public:
	VirtualMainThread();
	virtual ~VirtualMainThread();

	virtual void init( int loopnum);
	virtual void start();
	virtual void stop();

	void regist_syscmd( CommandBase* p);
	void regist_netcmd( CommandBase *p);
	void regist_usercmd( CommandBase *p);

	CommandBase* pop_one_cmd();

	int			 get_cmd_num();

protected:
	virtual void thread_worker() =0;

	void free_cmds();

protected:
	volatile bool		will_quit_;
	
	boost::shared_ptr<boost::thread>	thread_;
	int					loop_num_;

	COMMAND_LIST		system_cmds_[2];
	COMMAND_LIST		net_cmds_[2];
	COMMAND_LIST		user_cmds_[2];

	volatile int		cur_syscmds_, cur_netcmds_, cur_usercmds_;

	//app lock
	ThreadLock			lock_;
	ThreadLock			cmd_lock_;
};

#endif	//__VIRTUALMAINTHREAD_H__
