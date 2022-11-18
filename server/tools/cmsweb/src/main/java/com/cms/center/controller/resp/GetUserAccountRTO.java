package com.cms.center.controller.resp;

import java.io.Serializable;

/**
 * Created by lujf1978@163.com on 11/18/2022.
 *
 * @version 1.00
 */
public class GetUserAccountRTO implements Serializable
{
	private Long useriid;
	private String gates;

	public Long getUseriid()
	{
		return useriid;
	}

	public void setUseriid(Long useriid)
	{
		this.useriid = useriid;
	}

	public String getGates()
	{
		return gates;
	}

	public void setGates(String gates)
	{
		this.gates = gates;
	}
}
