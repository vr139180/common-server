package com.cms.commons.api.exception;

import com.cms.commons.api.APICallResult;

/**
 * Created by lujf1978@163.com on 2021/8/24.
 *
 * @version 1.00
 */
public class CustomResultException extends RuntimeException
{
	private APICallResult<?> result = null;

	public CustomResultException(){
		super();
	}

	public CustomResultException( APICallResult<?> err)
	{
		super();
		this.result = err;
	}

	public APICallResult<?> getResult(){
		return this.result;
	}
}
