package com.cms.commons.api.exception;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by lujf1978@163.com on 2020/10/14.
 *
 * @version 1.00
 */
public class ValidateParamException extends RuntimeException
{
	private List<String> errors = new ArrayList<>();

	public ValidateParamException(){
		super();
	}

	public ValidateParamException( List<String> err){
		super();
		this.errors = err;
	}

	public void addError( String error)
	{
		errors.add( error);
	}

	public List<String> getErrors(){ return this.errors;}
}
