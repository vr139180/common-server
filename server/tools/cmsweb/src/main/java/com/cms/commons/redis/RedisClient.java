package com.cms.commons.redis;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.stereotype.Component;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.exceptions.JedisException;

import java.io.IOException;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
@Component
public class RedisClient
{
	private static final Logger logger = LogManager.getLogger(RedisClient.class);

	public <T> T invoke(JedisPool pool, RedisClientInvoker<T> clients)
	{
		T obj = null;
		Jedis jedis = null;
		boolean broken = false;

		try
		{
			jedis = pool.getResource();
			obj = clients.invoke(jedis);
		}
		catch (JedisException | IOException ex) {
			logger.error(ex.getMessage(), ex);
		}
		finally
		{
			if( jedis != null)
			{
				if (jedis.isConnected())
					jedis.close();
			}
		}

		return obj;
	}
}
