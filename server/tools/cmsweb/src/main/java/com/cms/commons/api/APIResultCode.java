package com.cms.commons.api;

import com.alibaba.fastjson.annotation.JSONField;

import java.io.Serializable;

/**
 * Created by lujf1978@163.com on 2020/10/16.
 *
 * @version 1.00
 */
public class APIResultCode implements Serializable
{
	public static final APIResultCode
			SUCCESS = new APIResultCode( 0, "success"),
			SYSTEM_ERROR = new APIResultCode( -1, "系统错误"),
			PARAM_ERROR = new APIResultCode( -2, "参数解析错误"),
			DATABASE_ERROR = new APIResultCode( -3, "数据库错误"),
			REDIS_ERROR = new APIResultCode( -4, "redis错误"),
			ES_ERROR = new APIResultCode( -5, "es错误"),
			SERVICE_ERROR = new APIResultCode( -6, "微服务不可用"),
			OSS_ERROR = new APIResultCode( -7, "oss异常"),
			RABBIT_ERROR = new APIResultCode( -8, "消息队列异常"),
			APITOKEN_FAIL = new APIResultCode( -100, "api token错误");

	public APIResultCode(int v, String m)
	{
		this._msg = m;
		this._val = v;
	}

	public APIResultCode(){
		this._msg = "success";
		this._val = 0;
	}

	private int _val = 0;
	private String _msg = "";

	public int getCode() {
		return this._val;
	}
	public void setCode( int c){
		this._val = c;
	}

	public String getMessage(){
		return this._msg;
	}
	public void setMessage( String m){ this._msg = m;}

	@JSONField(serialize = false)
	public boolean isSuccess(){ return this._val == 0;}
}
