package com.cms.center.redis;

/**
 * Created by lujf1978@163.com on 2020/8/18.
 *
 * @version 1.00
 */
public final class RedisKeyConst
{
	//eureka master node
	public static final String EUREKA_MASTERNODE_KEY = "_EUREKA:MASTER";
	public static final String FIELD_EUREAKMASTER_IID = "iid";
	public static final String FIELD_EUREAKMASTER_SVR = "svrinfo";

	//gate list
	public static final String GATELIST_WS = "-GATE:WS-LIST";
	public static final String GATELIST_TCP = "-GATE:TCP-LIST";
	public static final String GATENODE_INFO = "-GATE:NODE%d";
}