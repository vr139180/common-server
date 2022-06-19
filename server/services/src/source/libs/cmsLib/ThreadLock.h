#ifndef __THREADLOCK_H__
#define __THREADLOCK_H__

#include <cmsLib/GlobalSettings.h>
#include <boost/thread/mutex.hpp>

class ThreadLock
{
    friend class ThreadLockWrapper;
public:

    ThreadLock();
    ~ThreadLock();

    void Lock();
    void Unlock();

protected:
	boost::mutex	mutex_;
};

class ThreadLockWrapper
{
public:
    ThreadLockWrapper( ThreadLock& lock):mLock(lock)
    {
        mLock.Lock();
    }

    ~ThreadLockWrapper()
    {
        mLock.Unlock();
    }

private:
    ThreadLock&     mLock;
};

#endif // __THREADLOCK_H__
