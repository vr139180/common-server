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