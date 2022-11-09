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

#include "cmsLib/system/VirtualMainThread.h"

#include <cmsLib/base/OSSystem.h>

VirtualMainThread::VirtualMainThread():
	will_quit_( false),
	loop_num_( 1)
{
	cur_netcmds_ =cur_syscmds_ =cur_usercmds_ =0;
}

VirtualMainThread::~VirtualMainThread()
{
	free_cmds();
}

void VirtualMainThread::free_cmds()
{
	ThreadLockWrapper guard( lock_);

	for( int ii =0; ii < 2; ++ii)
	{
		for( COMMAND_LIST_ITER iter =system_cmds_[ii].begin(); iter != system_cmds_[ii].end(); ++iter)
			delete (*iter);
		system_cmds_[ii].clear();

		for( COMMAND_LIST_ITER iter2 =net_cmds_[ii].begin(); iter2 != net_cmds_[ii].end(); ++iter2)
			delete (*iter2);
		net_cmds_[ii].clear();

		for( COMMAND_LIST_ITER iter3 =user_cmds_[ii].begin(); iter3 != user_cmds_[ii].end(); ++iter3)
			delete (*iter3);
		user_cmds_[ii].clear();
	}
}

int	VirtualMainThread::get_cmd_num()
{
	int cnt =0;

	for( int ii =0; ii < 2; ++ii)
	{
		cnt += (int)system_cmds_[ii].size();
		cnt += (int)net_cmds_[ii].size();
		cnt += (int)user_cmds_[ii].size();
	}

	return cnt;
}

CommandBase* VirtualMainThread::pop_one_cmd()
{
	if( will_quit_) return 0;

#define VMT_ANOTHER_CMD_LIST( cur)	(cur==1?0:1)

	CommandBase* pCmd =0;

	if( system_cmds_[cur_syscmds_].size() > 0)
	{
		pCmd =system_cmds_[cur_syscmds_].front();
		system_cmds_[cur_syscmds_].pop_front();
	}
	else if( net_cmds_[cur_netcmds_].size() > 0)
	{
		pCmd =net_cmds_[cur_netcmds_].front();
		net_cmds_[cur_netcmds_].pop_front();
	}
	else if( user_cmds_[cur_usercmds_].size() > 0)
	{
		pCmd =user_cmds_[cur_usercmds_].front();
		user_cmds_[cur_usercmds_].pop_front();
	}

	if( system_cmds_[cur_syscmds_].size() == 0)
	{
		if( system_cmds_[VMT_ANOTHER_CMD_LIST(cur_syscmds_)].size() > 0)
		{
			ThreadLockWrapper guard( lock_);

			cur_syscmds_ =VMT_ANOTHER_CMD_LIST(cur_syscmds_);
		}
		else if( net_cmds_[cur_netcmds_].size() == 0)
		{
			if( net_cmds_[VMT_ANOTHER_CMD_LIST(cur_netcmds_)].size() > 0)
			{
				ThreadLockWrapper guard( lock_);

				cur_netcmds_ =VMT_ANOTHER_CMD_LIST( cur_netcmds_);
			}
			else if( user_cmds_[cur_usercmds_].size() == 0)
			{
				if( user_cmds_[VMT_ANOTHER_CMD_LIST( cur_usercmds_)].size() > 0)
				{
					ThreadLockWrapper guard( lock_);

					cur_usercmds_ =VMT_ANOTHER_CMD_LIST( cur_usercmds_);
				}
			}
		}
	}

	return pCmd;
}

void VirtualMainThread::regist_syscmd( CommandBase* p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_) return;

	ThreadLockWrapper guard( lock_);
	system_cmds_[VMT_ANOTHER_CMD_LIST( cur_syscmds_)].push_back( p_p.release());
}

void VirtualMainThread::regist_netcmd( CommandBase *p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_) return;

	ThreadLockWrapper guard( lock_);

	net_cmds_[VMT_ANOTHER_CMD_LIST( cur_netcmds_)].push_back( p_p.release());
}

void VirtualMainThread::regist_usercmd( CommandBase *p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_) return;

	ThreadLockWrapper guard( lock_);

	user_cmds_[VMT_ANOTHER_CMD_LIST( cur_usercmds_)].push_back( p_p.release());
}

void VirtualMainThread::init( int loopnum)
{
	this->loop_num_ =loopnum;
}

void VirtualMainThread::start()
{
	stop();

	will_quit_ =false;

	boost::function0<void> f = boost::bind( &VirtualMainThread::thread_worker, this);
	thread_ =boost::shared_ptr<boost::thread>(new boost::thread( f));
}

void VirtualMainThread::stop()
{
	will_quit_ =true;

	if( thread_.get() == 0)
		return;

	thread_->join();
	thread_.reset<boost::thread>( 0);
}

