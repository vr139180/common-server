package com.cms.center.services;

import com.cms.center.controller.resp.EurekaMasterRTO;
import com.cms.center.redis.GateNodeInfo;

import java.util.List;

/**
 * Created by lujf1978@163.com on 11/18/2022.
 *
 * @version 1.00
 */
public interface EurekaService
{
	EurekaMasterRTO getEurekaMasterNode();
	//根据类型获取gate列表, type: 1 tcp 2 websocket
	List<GateNodeInfo> getGateNodeList( int type);
}
