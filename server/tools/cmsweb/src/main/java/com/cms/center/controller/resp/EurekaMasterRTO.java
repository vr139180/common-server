package com.cms.center.controller.resp;

import java.io.Serializable;

/**
 * Created by lujf1978@163.com on 11/18/2022.
 *
 * @version 1.00
 */
public class EurekaMasterRTO implements Serializable
{
	private Long iid;
	private String ip;
	private Integer port;
	private Long token;

	public Long getIid()
	{
		return iid;
	}

	public void setIid(Long iid)
	{
		this.iid = iid;
	}

	public String getIp()
	{
		return ip;
	}

	public void setIp(String ip)
	{
		this.ip = ip;
	}

	public Integer getPort()
	{
		return port;
	}

	public void setPort(Integer port)
	{
		this.port = port;
	}

	public Long getToken()
	{
		return token;
	}

	public void setToken(Long token)
	{
		this.token = token;
	}
}
