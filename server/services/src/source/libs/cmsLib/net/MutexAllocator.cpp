#include <cmsLib/net/MutexAllocator.h>
#include <cmsLib/ThreadLock.h>

MutexAllocator& MutexAllocator::getInstance()
{
	static MutexAllocator s_instance_;

	return s_instance_;
}

MutexAllocator::MutexDescription::MutexDescription()
:reference_(0)
{
}

MutexAllocator::MutexDescription::~MutexDescription()
{
}

MutexAllocator::MutexAllocator()
:mutex_size_(0)
{
	mutexs_.reset();
}

MutexAllocator::~MutexAllocator()
{
}

void MutexAllocator::init_allocator( int size)
{
	assert(size > 0 );
	mutex_size_ =size;
	mutexs_.reset( new MutexDescription[mutex_size_]);
}

void MutexAllocator::alloc_one( std::vector<ThreadLock*>& locks)
{
	ThreadLockWrapper	guard( allocator_mutex_);

	int domutx =0;
	for( int minm =mutexs_[0].reference_, ii =1; ii < mutex_size_; ++ii)
	{
		if( minm > mutexs_[ii].reference_)
		{
			domutx =ii;
			minm =mutexs_[ii].reference_;
		}
	}

	locks.push_back( &(mutexs_[domutx].mutex_));
	locks.push_back( &(mutexs_[domutx].queue_mutex_));
}

void MutexAllocator::free_one( ThreadLock * m)
{
	ThreadLockWrapper	guard( allocator_mutex_);

	for( int ii =0; ii < mutex_size_; ++ii)
	{
		if( &(mutexs_[ii].mutex_) == m)
		{
			-- mutexs_[ii].reference_;
			break;
		}
	}
}

