package com.cms.commons.redis;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.cache.annotation.CachingConfigurerSupport;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
@Configuration
public class JedisConfig extends CachingConfigurerSupport
{
	@Autowired
	private RedisConfig redisConfigProperties;

	@Bean(name = "jedisPoolConfig")
	@ConfigurationProperties(prefix = "spring.redis.jedis.pool")
	public JedisPoolConfig getRedisConfig()
	{
		JedisPoolConfig config = new JedisPoolConfig();
		return config;
	}

	@Bean(name = "jedisPool")
	public JedisPool jedisPool(@Qualifier(value = "jedisPoolConfig") final JedisPoolConfig jedisPoolConfig)
	{
		String host = redisConfigProperties.getHost();
		Integer timeout = redisConfigProperties.getTimeout();
		int port = redisConfigProperties.getPort();
		String password = redisConfigProperties.getPassword();
		int database = redisConfigProperties.getDatabase();
		JedisPool jedisPool = new JedisPool(jedisPoolConfig, host, port, timeout, password, database);

		return jedisPool;
	}
}
