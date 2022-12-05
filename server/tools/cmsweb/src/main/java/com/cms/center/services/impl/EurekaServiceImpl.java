package com.cms.center.services.impl;

import com.cms.center.controller.resp.EurekaMasterRTO;
import com.cms.center.redis.GateNodeInfo;
import com.cms.center.redis.RedisKeyConst;
import com.cms.center.services.EurekaService;
import com.cms.commons.redis.RedisCache;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

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

	@Override
	public List<GateNodeInfo> getGateNodeList(int type)
	{
		List<GateNodeInfo> dat = new ArrayList<>();

		String key = RedisKeyConst.GATELIST_WS;
		if( type == 1)
			key = RedisKeyConst.GATELIST_TCP;

		List<Long> gateiids = redisUtil.zRangeByScore( key, Long.class, 0.0, 100000.0);
		if( gateiids.size() == 0)
			return dat;

		for( Long iid : gateiids)
		{
			String rkey = String.format( RedisKeyConst.GATENODE_INFO, iid);
			GateNodeInfo gi = redisUtil.get(rkey, GateNodeInfo.class);
			if( gi == null)
			{
				//gate已经失效
				redisUtil.zRem( key, iid);
				continue;
			}

			dat.add( gi);
		}

		return dat;
	}
}
