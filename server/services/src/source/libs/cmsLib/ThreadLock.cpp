#include <cmsLib/ThreadLock.h>
#include <cmsLib/GlobalSettings.h>

#include <boost/static_assert.hpp>

ThreadLock::ThreadLock()
{
}

ThreadLock::~ThreadLock()
{
}

void ThreadLock::Lock()
{
	mutex_.lock();
}

void ThreadLock::Unlock()
{
	mutex_.unlock();
}
