package com.cms.center.services.impl;

import com.cms.center.controller.resp.EurekaMasterRTO;
import com.cms.center.redis.RedisKeyConst;
import com.cms.center.services.EurekaService;
import com.cms.commons.redis.RedisCache;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by lujf1978@163.com on 11/18/2022.
 *
 * @version 1.00
 */
@Service
public class EurekaServiceImpl implements EurekaService
{
	@Autowired
	private RedisCache redisUtil;

	@Override
	public EurekaMasterRTO getEurekaMasterNode()
	{
		return redisUtil.getHash(RedisKeyConst.EUREKA_MASTERNODE_KEY, RedisKeyConst.FIELD_EUREAKMASTER_SVR, EurekaMasterRTO.class);
	}
}
