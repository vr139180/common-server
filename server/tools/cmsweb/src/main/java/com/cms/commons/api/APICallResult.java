package com.cms.commons.api;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.TypeReference;
import com.alibaba.fastjson.annotation.JSONField;

import java.io.Serializable;

/**
 * Created by lujf1978@163.com on 2020/10/13.
 *
 * @version 1.00
 */
public class APICallResult<T> implements Serializable
{
	private APIResultCode code = null;
	private T data = null;

	public APICallResult()
	{
		int ii =0;
	}

	public APICallResult( APIResultCode code)
	{
		this.code = code;
	}

	public APICallResult( APIResultCode code, T data)
	{
		this.code = code;
		this.data = data;
	}

	public void setData( T d) { this.data = d;}

	public void setResult( APIResultCode c, T d){
		this.code = c;
		if( code.getCode() != 0)
			this.data = null;
		else
			this.data = d;
	}

	@JSONField(serialize = false)
	public boolean isSuccess(){ return code.isSuccess();}

	@JSONField(serialize = false)
	public APIResultCode getAPIResultCode(){ return code;}

	public int getCode(){ return code.getCode();}
	public void setCode( int c){
		if( code == null)
			code = new APIResultCode();
		code.setCode( c);
	}

	public String getMessage(){ return code.getMessage();}
	public void setMessage( String m){
		if( code == null)
			code = new APIResultCode();
		code.setMessage( m);
	}

	public T getData(){ return data;}

	@Override
	public String toString()
	{
		return "APICallResult{" +
				"code=" + code.getCode() +
				", message=" + code.getMessage() +
				'}';
	}

	public static void main( String[] args)
	{
		String str = "{\"code\":\"-1\",\"message\":\"test\",\"data\":{\"code\":\"-1\",\"message\":\"test\"}}";
		//APICallResult<String> r = JSONUtil.jsonStrToObj();
		//JSONObject r = JSONUtil.jsonStrToObj("{\"code\":\"-1\",\"message\":\"test\",\"data\":{\"code\":\"-1\",\"message\":\"test\"}}");
		try{
			APICallResult<APIResultCode> r = JSON.parseObject( str, new TypeReference<APICallResult<APIResultCode>>(){});
			System.out.print(r.getCode());
		}
		catch(Exception e){
			System.out.print(e.toString());
		}
		int i=0;
		++i;
	}
}
