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

#ifndef __THREADCONTEXTPOOL_H__
#define __THREADCONTEXTPOOL_H__

#include <cmsLib/cmsLib.h>
#include <cmsLib/base/EwwSystem.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/jobsystem/ThreadContext.h>

#include <list>

template< class T>
class ThreadContextPool
{
public:
	ThreadContextPool();
	~ThreadContextPool();

	bool init_pool( int waterline);

	ThreadContext*	get_free_context();
	void release_context( ThreadContext* p);

	std::list<ThreadContext*>	get_all_context(){
		return all_context_;
	}

protected:
	ThreadLock							thread_lock_;
	std::list<ThreadContext*>			all_context_;
	std::list<ThreadContext*>			free_all_context_;

	int									waterline_;
};

template< class T>
ThreadContextPool<T>::ThreadContextPool():
	waterline_( 1)
{
}

template< class T>
ThreadContextPool<T>::~ThreadContextPool()
{
	ThreadLockWrapper guard( thread_lock_);

	std::list<ThreadContext*>::iterator iter =all_context_.begin();
	for( ; iter != all_context_.end(); ++iter)
		delete (*iter);

	all_context_.clear();
	free_all_context_.clear();
}

template< class T>
bool ThreadContextPool<T>::init_pool( int waterline)
{
	waterline_ =waterline;

	if( waterline_ <= 0)
		waterline_ =1;

	{
		ThreadLockWrapper guard( thread_lock_);
		for( int ii =0; ii < waterline_; ++ii)
		{
			T* c =new T();
			all_context_.push_back( c);
			free_all_context_.push_back( c);
		}
	}

	return true;
}

template< class T>
ThreadContext* ThreadContextPool<T>::get_free_context()
{
	ThreadLockWrapper guard( thread_lock_);
	
	if( free_all_context_.size() == 0)
	{
		T* c =new T();
		all_context_.push_back( c);
		return c;
	}

	ThreadContext* c =free_all_context_.front();
	free_all_context_.pop_front();

	return c;
}

template< class T>
void ThreadContextPool<T>::release_context( ThreadContext* p)
{
	if( p == 0)
		return;

	ThreadLockWrapper guard( thread_lock_);

	free_all_context_.push_back( p);
	free_all_context_.unique();

	int rmnum =(int)all_context_.size() - waterline_;
	if( rmnum > 0 && free_all_context_.size() > 0)
	{
		for( int ii =0; free_all_context_.size() > 0 && ii < rmnum; ++ii)
		{
			ThreadContext* p =free_all_context_.front();
			free_all_context_.pop_front();
			delete p;
		}
	}
}

#endif	//__THREADCONTEXTPOOL_H__
