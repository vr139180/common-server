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
