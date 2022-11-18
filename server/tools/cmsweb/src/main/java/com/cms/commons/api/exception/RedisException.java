package com.cms.commons.api.exception;

/**
 * Created by lujf1978@163.com on 2020/10/23.
 *
 * @version 1.00
 */
public class RedisException extends RuntimeException
{
	public RedisException(){
		super();
	}

	public RedisException( String msg){
		super( msg);
	}
}