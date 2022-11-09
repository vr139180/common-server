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

#ifndef __REDISPROTOBUFTHREADCACHE_H__
#define __REDISPROTOBUFTHREADCACHE_H__

#include <cmsLib/net/NetSession.h>

class RedisProtoBufThreadCache
{
public:
	RedisProtoBufThreadCache();
	~RedisProtoBufThreadCache();

	char* data() { return buf_; }
	int size() { return NET_SINGLE_PACKAGE_SIZE_MAX; }

private:
	char* buf_;
};

inline
RedisProtoBufThreadCache::RedisProtoBufThreadCache()
{
	buf_ = new char[NET_SINGLE_PACKAGE_SIZE_MAX];
}

inline
RedisProtoBufThreadCache::~RedisProtoBufThreadCache()
{
	delete []buf_;
}

#endif //__REDISPROTOBUFTHREADCACHE_H__