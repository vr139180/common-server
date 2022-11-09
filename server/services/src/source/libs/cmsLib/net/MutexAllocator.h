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

#ifndef __MUTEXTALLOCATOR__H__
#define __MUTEXTALLOCATOR__H__

class ThreadLock;

#include <boost/smart_ptr/scoped_array.hpp>
#include <cmsLib/ThreadLock.h>
#include <vector>

class MutexAllocator
{
public:
	class MutexDescription
	{
	public:
		MutexDescription();
		virtual ~MutexDescription();

		operator ThreadLock* (){
			++reference_;
			return &mutex_;
		};

		ThreadLock	mutex_;
		ThreadLock	queue_mutex_;
		int	reference_;
	};

private:
	MutexAllocator();

public:
	static MutexAllocator& getInstance();
	
	virtual ~MutexAllocator();

	void init_allocator( int size);

	void alloc_one( std::vector<ThreadLock*>& locks);

	void free_one( ThreadLock * m);

private:
	boost::scoped_array<MutexDescription>	mutexs_;
	int										mutex_size_;

	ThreadLock								allocator_mutex_;
};

#endif //__MUTEXTALLOCATOR__H__
