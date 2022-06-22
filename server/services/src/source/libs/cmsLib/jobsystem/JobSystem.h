#ifndef __JOBSYSTEM_H__
#define __JOBSYSTEM_H__

#include <cmsLib/cmsLib.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/Log.h>
#include <boost/thread.hpp>
#include <cmsLib/ThreadLock.h>

#include <cmsLib/jobsystem/JobMTBase.h>

template< class TF>
class JobSystem
{
	class JobThread
	{
	public:
		JobThread():parent_(0){}

		void start_thread( JobSystem<TF>* p){
			parent_ =p;

			boost::function0<void> f = boost::bind( &JobSystem<TF>::thread_worker, parent_);
			thread_ =boost::shared_ptr<boost::thread>(new boost::thread( f));
		}

		void exit_thread()
		{
			if( thread_.get() != 0)
				thread_->join();
		}

	private:
		boost::shared_ptr<boost::thread>	thread_;
		JobSystem<TF>*						parent_;
	};

public:
	static JobSystem<TF>& get_instance();

	JobSystem();

	bool init_jobsystem( int threadnum);
	void uninit_jobsystem();

	void							do_mainthread();

	void							push_threadcmd( JobMTBase* cmd);
	JobMTBase*						pop_threadcmd();

	void							push_maincmd( JobMTBase* cmd);
	JobMTBase*						pop_maincmd();

	ThreadContext*					get_new_threadcontext( int type);
	void							free_threadcontext( int type, ThreadContext* p);

	int	get_threadnum(){ return thread_nums_; }

	int get_thread_queue_size();

protected:
	void	thread_worker();

protected:
	ThreadLock						thread_lock_;
	std::deque<JobMTBase*>			thread_queue_;

	ThreadLock						main_lock_;
	std::deque<JobMTBase*>			main_queue_;

	bool							b_exit_thread_;
	int								thread_nums_;
	boost::scoped_array<JobThread>	threads_;

	TF								thread_context_factory_;
};

template< class TF>
JobSystem<TF>::JobSystem():
	thread_nums_( 0)
{
}

template< class TF>
bool JobSystem<TF>::init_jobsystem( int threadnum)
{
	b_exit_thread_ =false;
	thread_nums_ =threadnum;

	thread_context_factory_.init_factory( thread_nums_);

	threads_.reset( new JobThread[thread_nums_]);
	for( int ii =0; ii < thread_nums_; ++ii)
		threads_[ii].start_thread( this);

	return true;
}

template< class TF>
void JobSystem<TF>::uninit_jobsystem()
{
	b_exit_thread_ =true;

	for( int ii =0; ii < thread_nums_; ++ii)
		threads_[ii].exit_thread();
}

template< class TF>
void JobSystem<TF>::push_threadcmd( JobMTBase* cmd)
{
	ThreadLockWrapper guard( thread_lock_);

	thread_queue_.push_back( cmd);
}

template< class TF>
JobMTBase* JobSystem<TF>::pop_threadcmd()
{
	if( thread_queue_.size() == 0)
		return 0;

	JobMTBase* ret =0;

	{
		ThreadLockWrapper guard( thread_lock_);

		if( thread_queue_.size() == 0)
			return 0;

		ret =thread_queue_.front();
		thread_queue_.pop_front();
	}

	return ret;
}

template< class TF>
int JobSystem<TF>::get_thread_queue_size()
{
	return (int)thread_queue_.size();
}

template< class TF>
void JobSystem<TF>::push_maincmd( JobMTBase* cmd)
{
	ThreadLockWrapper guard( main_lock_);

	main_queue_.push_back( cmd);
}

template< class TF>
JobMTBase* JobSystem<TF>::pop_maincmd()
{
	if( main_queue_.size() == 0)
		return 0;

	JobMTBase* ret =0;

	{
		ThreadLockWrapper guard( main_lock_);

		if( main_queue_.size() == 0)
			return 0;

		ret =main_queue_.front();
		main_queue_.pop_front();
	}

	return ret;
}

template< class TF>
void JobSystem<TF>::do_mainthread()
{
	if( main_queue_.size() == 0)
		return;

	JobMTBase* pcmd =pop_maincmd();

	std::unique_ptr<JobMTBase> a_pcmd( pcmd);

	if( pcmd == 0)
		return;

	pcmd->run_main();
}

template< class TF>
void JobSystem<TF>::thread_worker()
{
	while( !b_exit_thread_)
	{
		OSSystem::mOS->thread_yield();

		JobMTBase* cmd = pop_threadcmd();

		if( cmd == 0)
			continue;

		ThreadContext* p =thread_context_factory_.get_threadcontext( cmd->get_threadcontext_type());
		cmd->run_thread( p);
		thread_context_factory_.release_threadcontext( cmd->get_threadcontext_type(), p);

		//add to main thread queue
		push_maincmd( cmd);
	}
}

template< class TF>
ThreadContext* JobSystem<TF>::get_new_threadcontext( int type)
{
	return thread_context_factory_.get_threadcontext( type);
}

template< class TF>
void JobSystem<TF>::free_threadcontext( int type, ThreadContext* p)
{
	thread_context_factory_.release_threadcontext( type, p);
}

template< class TF>
JobSystem<TF>& JobSystem<TF>::get_instance()
{
	static JobSystem<TF>	s_instance_;
	return s_instance_;
}

#endif	//__JOBSYSTEM_H__
