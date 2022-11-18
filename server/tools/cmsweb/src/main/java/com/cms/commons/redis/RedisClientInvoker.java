package com.cms.commons.redis;

import redis.clients.jedis.Jedis;

import java.io.IOException;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
public interface RedisClientInvoker<T>
{
	T invoke( Jedis jedis) throws IOException;
}
