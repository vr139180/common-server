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

#ifndef __TIMERCONTAINER__H__
#define __TIMERCONTAINER__H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/core_type.h>
#include <cmsLib/base/OSSystem.h>

#pragma pack( push, 8)
#include <boost/function.hpp>
#pragma  pack( pop)

#include <list>
#include <map>

typedef boost::function<void( u64, int interval, u64 iid, bool& finish)>	APPTIMER_FUN_MAP;

struct TimerKey
{
public:
	static TimerKey	None;

	TimerKey(){
		step =-1;
		tid =0;
	}

	TimerKey( int step, u64 tid){
		this->step =step;
		this->tid =tid;
	}

	bool operator == ( const TimerKey& o ) const{
		return step == o.step && tid == o.tid;
	}

	bool is_none(){
		return step == -1 && tid == 0;
	}

	bool is_validate(){
		return !is_none();
	}

public:
	int		step;
	u64		tid;
};

class TimerContainer
{
private:
	struct timer_object
	{
		timer_object( u64 id, APPTIMER_FUN_MAP f):
			before_node_(0),next_node_(0), iid_(id), fun_(f)
		{
			last_time_ = OSSystem::mOS->GetTimestamp();
		}

		bool is_timeout( u64 tnow, int interval){
			if( last_time_ + interval > tnow)
				return false;

			last_time_ =tnow;
			return true;
		}

		timer_object			*before_node_;
		timer_object			*next_node_;

		u64						iid_;
		APPTIMER_FUN_MAP		fun_;
		u64						last_time_;
	};

	class TimerSequence
	{
	public:
		TimerSequence( int t);
		~TimerSequence();

		u64 add_timer( APPTIMER_FUN_MAP f);
		void del_timer( u64 iid);

		void do_timer( u64 tnow);

	protected:
		u64 generate_timerid(){ return timer_id_generator_++;}

		void release();

	private:
		//header and tail
		timer_object*	header_;
		timer_object*	tail_;

		std::map<u64,timer_object*>		timers_;

		int		time_inverval_;
		u64		timer_id_generator_;
	};
	
public:
	typedef std::map< int, TimerSequence>	TIMER_MAP;

public:
	TimerContainer();
	~TimerContainer();

	void reset();

	TimerKey add_timer( int step, APPTIMER_FUN_MAP f);

	void del_timer( TimerKey tid);

	void delall_timer();

	void timer_tick();
	void timer_tick( u64 now_t);

private:
	TIMER_MAP		timers_;
};

#endif	//__TIMERCONTAINER__H__
