#ifndef __SERVERAPPBASE_H__
#define __SERVERAPPBASE_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/system/CommandBase.h>
#include <cmsLib/system/TimerContainer.h>
#include <cmsLib/ThreadLock.h>

class ServerAppBase
{
	friend class ServerAppWrapper;

public:
	typedef std::list<CommandBase* >			COMMAND_LIST;
	typedef std::list<CommandBase* >::iterator	COMMAND_LIST_ITER;

public:
	ServerAppBase();

    void                SetCmdLine( s32 argc, char* argv[] );
    s32                 GetArgC() const;
    char**              GetArgV() const;

	virtual void main_loop() =0;

	void regist_syscmd( CommandBase* p);
	void regist_netcmd( CommandBase *p);
	void regist_usercmd( CommandBase *p);

	virtual CommandBase* pop_one_cmd();
	virtual CommandBase* pop_net_cmd();
	virtual CommandBase* pop_sys_cmd();

	int			 get_cmd_num();

	void apptimer_reset(){ app_timer_.reset(); }

	TimerKey add_apptimer( int step, APPTIMER_FUN_MAP f){ return app_timer_.add_timer( step, f); }

	void del_apptimer( TimerKey tid){ app_timer_.del_timer( tid); }

	void delall_moduletimer(){ app_timer_.delall_timer(); }

	void quit_app(){ will_quit_app_ = true; }

	ThreadLock& get_threadlock() { return this->lock_; }

protected:
	virtual bool load_config() { return false; }
	virtual bool pre_init(){ return true; }
	virtual bool init_network(){ return true;}
    virtual bool init_database(){ return true; };
	virtual void uninit_network(){}
    virtual void uninit_database(){}

	virtual bool init_finish(){ return true;}
	virtual void uninit();

	virtual void register_timer(){}

	void free_cmds();

protected:
	volatile bool		will_quit_app_;

	s32                 mArgc;
	char**				mArgv;

	TimerContainer		app_timer_;

	COMMAND_LIST		system_cmds_[2];
	COMMAND_LIST		net_cmds_[2];
	COMMAND_LIST		user_cmds_[2];

	volatile int		cur_syscmds_, cur_netcmds_, cur_usercmds_;

	//app lock
	ThreadLock			lock_;
};


#endif // __SERVERAPPBASE_H__
