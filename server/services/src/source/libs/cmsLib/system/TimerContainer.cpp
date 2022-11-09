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

#include <cmsLib/system/TimerContainer.h>
#include <cmsLib/base/OSSystem.h>

TimerContainer::TimerSequence::TimerSequence( int t):
	timer_id_generator_( 1),
	time_inverval_( t),
	header_( 0),
	tail_( 0)
{
}

TimerContainer::TimerSequence::~TimerSequence()
{
	release();
}

void TimerContainer::TimerSequence::release()
{
	std::map<u64,timer_object*>::iterator iter =timers_.begin();
	for( ; iter != timers_.end(); ++iter)
		delete iter->second;
	timers_.clear();

	header_ =0;
	tail_ =0;
}

u64 TimerContainer::TimerSequence::add_timer( APPTIMER_FUN_MAP f)
{
	u64 tid =generate_timerid();
	timer_object* tobj =new timer_object( tid, f);

	timers_[tobj->iid_] =tobj;

	if( header_ == 0)
		tail_ =header_ =tobj;
	else
	{
		tail_->next_node_ =tobj;
		tobj->before_node_ =tail_;
		tail_ =tobj;
	}

	return tid;
}

void TimerContainer::TimerSequence::del_timer( u64 iid)
{
	std::map<u64,timer_object*>::iterator fiter =timers_.find( iid);
	if( fiter == timers_.end())
		return;

	timer_object* tobj =fiter->second;
	//remove from map
	timers_.erase( fiter);

	//remove from list
	timer_object* bobj =tobj->before_node_;
	timer_object* nobj =tobj->next_node_;
	if( bobj != 0)
		bobj->next_node_ =nobj;
	if( nobj != 0)
		nobj->before_node_ =bobj;

	if( header_ == tobj)
		header_ =nobj;
	if( tail_ == tobj)
		tail_ =bobj;

	delete tobj;
}

void TimerContainer::TimerSequence::do_timer( u64 tnow)
{
	size_t cnt =timers_.size();
	size_t icount =0;
	while( header_ != 0 && icount < cnt)
	{
		++icount;

		if( !header_->is_timeout( tnow, time_inverval_))
			break;

		bool bfinish =false;
		u64 oldid =header_->iid_;
		header_->fun_( tnow, time_inverval_, header_->iid_, bfinish);

		//remove timer
		if( header_ == 0)
			break;

		//check the header is the old header
		if( bfinish && header_->iid_ == oldid)
		{
			del_timer( header_->iid_);
			continue;
		}

		if( header_ != tail_)
		{
			timer_object* nobj =header_->next_node_;
			nobj->before_node_ =0;
			header_->next_node_ =0;

			header_->before_node_ =tail_;
			tail_->next_node_ =header_;

			tail_ =header_;
			header_ =nobj;
		}

	}
}

//-------------------------------------------------------------------------------
TimerKey TimerKey::None( -1, 0);

TimerContainer::TimerContainer()
{
}

TimerContainer::~TimerContainer()
{
}

void TimerContainer::reset()
{
	timers_.clear();
}

void TimerContainer::timer_tick()
{
	u64 now_t = OSSystem::mOS->GetTicks();

	timer_tick( now_t);
}

void TimerContainer::timer_tick( u64 now_t)
{
	for( TIMER_MAP::iterator iter =timers_.begin(); iter != timers_.end(); ++iter)
	{
		TimerSequence& obj =iter->second;
		obj.do_timer( now_t);
	}
}

TimerKey TimerContainer::add_timer( int step, APPTIMER_FUN_MAP f)
{
	if( step <= 0)
		return TimerKey::None;

	TIMER_MAP::iterator fiter =timers_.find( step);
	if( fiter == timers_.end())
	{
		timers_.insert( std::make_pair( step, TimerSequence( step)));
		fiter =timers_.find( step);
	}

	TimerSequence& ts =fiter->second;
	u64 iid =ts.add_timer( f);

	return TimerKey( step, iid);
}

void TimerContainer::del_timer( TimerKey tid)
{
	TIMER_MAP::iterator fiter =timers_.find( tid.step);
	if( fiter == timers_.end())
		return;

	TimerSequence& ts =fiter->second;
	ts.del_timer( tid.tid);
}

void TimerContainer::delall_timer()
{
	timers_.clear();
}
