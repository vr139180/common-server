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

#include <cmsLib/ServerAppBase.h>

ServerAppBase::ServerAppBase():
will_quit_app_( false)
{
	cur_netcmds_ =cur_syscmds_ =cur_usercmds_ =0;
}

void ServerAppBase::uninit()
{
	apptimer_reset();

	free_cmds();
}

void ServerAppBase::free_cmds()
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

int	ServerAppBase::get_cmd_num()
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

CommandBase* ServerAppBase::pop_one_cmd()
{
	if( will_quit_app_) return 0;

#define ANOTHER_CMD_LIST( cur)	(cur==1?0:1)

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
		if( system_cmds_[ANOTHER_CMD_LIST(cur_syscmds_)].size() > 0)
		{
			ThreadLockWrapper guard( lock_);

			cur_syscmds_ =ANOTHER_CMD_LIST(cur_syscmds_);
		}
		else if( net_cmds_[cur_netcmds_].size() == 0)
		{
			if( net_cmds_[ANOTHER_CMD_LIST(cur_netcmds_)].size() > 0)
			{
				ThreadLockWrapper guard( lock_);

				cur_netcmds_ =ANOTHER_CMD_LIST( cur_netcmds_);
			}
			else if( user_cmds_[cur_usercmds_].size() == 0)
			{
				if( user_cmds_[ANOTHER_CMD_LIST( cur_usercmds_)].size() > 0)
				{
					ThreadLockWrapper guard( lock_);

					cur_usercmds_ =ANOTHER_CMD_LIST( cur_usercmds_);
				}
			}
		}
	}

	return pCmd;
}

CommandBase* ServerAppBase::pop_net_cmd()
{
	if (will_quit_app_) return 0;

#define ANOTHER_CMD_LIST( cur)	(cur==1?0:1)

	CommandBase* pCmd = 0;

	if (net_cmds_[cur_netcmds_].size() > 0)
	{
		pCmd = net_cmds_[cur_netcmds_].front();
		net_cmds_[cur_netcmds_].pop_front();
	}
	else if (user_cmds_[cur_usercmds_].size() > 0)
	{
		pCmd = user_cmds_[cur_usercmds_].front();
		user_cmds_[cur_usercmds_].pop_front();
	}

	if (net_cmds_[cur_netcmds_].size() == 0)
	{
		if (net_cmds_[ANOTHER_CMD_LIST(cur_netcmds_)].size() > 0)
		{
			ThreadLockWrapper guard(lock_);

			cur_netcmds_ = ANOTHER_CMD_LIST(cur_netcmds_);
		}
		else if (user_cmds_[cur_usercmds_].size() == 0)
		{
			if (user_cmds_[ANOTHER_CMD_LIST(cur_usercmds_)].size() > 0)
			{
				ThreadLockWrapper guard(lock_);

				cur_usercmds_ = ANOTHER_CMD_LIST(cur_usercmds_);
			}
		}
	}

	return pCmd;
}

CommandBase* ServerAppBase::pop_sys_cmd()
{
	if (will_quit_app_) return 0;

#define ANOTHER_CMD_LIST( cur)	(cur==1?0:1)

	CommandBase* pCmd = 0;

	if (system_cmds_[cur_syscmds_].size() > 0)
	{
		pCmd = system_cmds_[cur_syscmds_].front();
		system_cmds_[cur_syscmds_].pop_front();
	}

	if (system_cmds_[cur_syscmds_].size() == 0)
	{
		if (system_cmds_[ANOTHER_CMD_LIST(cur_syscmds_)].size() > 0)
		{
			ThreadLockWrapper guard(lock_);

			cur_syscmds_ = ANOTHER_CMD_LIST(cur_syscmds_);
		}
	}

	return pCmd;
}

void ServerAppBase::regist_syscmd( CommandBase* p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_app_) return;

	ThreadLockWrapper guard( lock_);
	system_cmds_[ANOTHER_CMD_LIST( cur_syscmds_)].push_back( p_p.release());
}

void ServerAppBase::regist_netcmd( CommandBase *p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_app_) return;

	ThreadLockWrapper guard( lock_);

	net_cmds_[ANOTHER_CMD_LIST( cur_netcmds_)].push_back( p_p.release());
}

void ServerAppBase::regist_usercmd( CommandBase *p)
{
	std::unique_ptr<CommandBase> p_p( p);

	if( will_quit_app_) return;

	ThreadLockWrapper guard( lock_);

	user_cmds_[ANOTHER_CMD_LIST( cur_usercmds_)].push_back( p_p.release());
}

void ServerAppBase::SetCmdLine( s32 argc, char* argv[] )
{
	mArgc = argc;
	mArgv = argv;
}

s32 ServerAppBase::GetArgC() const
{
	return mArgc;
}

char** ServerAppBase::GetArgV() const
{
	return mArgv;
}
