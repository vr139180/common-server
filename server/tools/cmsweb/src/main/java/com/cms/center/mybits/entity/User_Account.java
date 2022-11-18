package com.cms.center.mybits.entity;

import java.io.Serializable;

/**
 * Created by lujf1978@163.com on 2020/9/2.
 *
 * @version 1.00
 */
public class User_Account implements Serializable
{
	private Long user_iid;
	private String account;
	private String pwd;
	private Integer state;

	public Long getUser_iid()
	{
		return user_iid;
	}

	public void setUser_iid(Long user_iid)
	{
		this.user_iid = user_iid;
	}

	public String getAccount()
	{
		return account;
	}

	public void setAccount(String account)
	{
		this.account = account;
	}

	public String getPwd()
	{
		return pwd;
	}

	public void setPwd(String pwd)
	{
		this.pwd = pwd;
	}

	public Integer getState()
	{
		return state;
	}

	public void setState(Integer state)
	{
		this.state = state;
	}
}
