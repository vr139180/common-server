package com.cms.commons.api.exception;

/**
 * Created by lujf1978@163.com on 2020/10/18.
 *
 * @version 1.00
 */
public class DatabaseException extends RuntimeException
{
	public DatabaseException(){
		super();
	}

	public DatabaseException( String msg){
		super( msg);
	}
}
