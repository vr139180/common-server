package com.cms.commons.utils;

import com.alibaba.fastjson.*;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by Administrator on 2020/8/18.
 */
public class JSONUtil
{
	public static String toString( JSONObject obj, String key, String def)
	{
		try{
			String ret = obj.getString( key);
			if( ret == null)
				return def;
			return ret;
		}
		catch(JSONException e){
			return def;
		}
	}

	public static String toString( JSONObject obj, String key)
	{
		return toString( obj, key, "");
	}

	public static String toNullStringIfDef( JSONObject obj, String key, String def)
	{
		String v = toString( obj, key, def);
		if( def == null)
		{
			if( v == null)
				return null;
			else
				return v;
		}
		else
		{
			if( v == null)
				return null;

			if( def.compareTo( v) == 0)
				return null;
		}

		return v;
	}

	public static Short toShort( JSONObject obj, String key, int def)
	{
		try{
			Short ret = obj.getShort(key);
			if( ret == null)
				return (short)def;
			return ret;
		}
		catch(JSONException e){
			return (short)def;
		}
	}

	public static Short toShort( JSONObject obj, String key)
	{
		return toShort( obj, key, 0);
	}

	public static Short toNullShortIfDef( JSONObject obj, String key, int def)
	{
		short v = toShort( obj, key, def);
		if( v == (short)def)
			return null;

		return v;
	}

	public static Integer toInteger( JSONObject obj, String key, int def)
	{
		try{
			Integer ret = obj.getInteger(key);
			if( ret == null)
				return def;
			return ret;
		}
		catch(JSONException e){
			return def;
		}
	}

	public static Integer toInteger( JSONObject obj, String key)
	{
		return toInteger( obj, key, 0);
	}

	public static Integer toNullIntegerIfDef( JSONObject obj, String key, int def)
	{
		int v = toInteger( obj, key, def);
		if( v == def)
			return null;

		return v;
	}

	public static Date toDate( JSONObject obj, String key, Date def)
	{
		try{
			Date ret = obj.getDate(key);
			if( ret == null)
				return def;
			return ret;
		}
		catch(JSONException e){
			return def;
		}
	}

	public static Date toDate( JSONObject obj, String key)
	{
		return toDate( obj, key, null);
	}

	public static BigDecimal toDecimal( JSONObject obj, String key, double def)
	{
		try{
			BigDecimal ret = obj.getBigDecimal(key);
			if( ret == null)
				return new BigDecimal(def);
			return ret;
		}
		catch(JSONException e){
			return new BigDecimal(def);
		}
	}

	public static BigDecimal toNullBigDecimalIfDef( JSONObject obj, String key, double def)
	{
		BigDecimal v = toDecimal(obj, key, def);
		if( v.doubleValue() == def)
			return null;

		return v;
	}

	public static BigDecimal toDecimal( JSONObject obj, String key)
	{
		return toDecimal( obj, key, 0.0f);
	}

	public static BigDecimal AddDecimalAB( BigDecimal a, BigDecimal b)
	{
		BigDecimal ret = BigDecimal.valueOf(a.doubleValue());
		return ret.add(b);
	}

	public static BigDecimal subDecimalAB( BigDecimal a, BigDecimal b)
	{
		BigDecimal ret = BigDecimal.valueOf(a.doubleValue());
		return ret.subtract(b);
	}

	public static BigDecimal multiplyDecimalAB( BigDecimal a, BigDecimal b)
	{
		BigDecimal ret = BigDecimal.valueOf(a.doubleValue());
		return ret.multiply(b);
	}

	public static BigDecimal multiplyDecimalAB( BigDecimal a, int b)
	{
		BigDecimal ret = BigDecimal.valueOf(a.doubleValue());
		BigDecimal v = new BigDecimal( b);
		return ret.multiply(v);
	}

	public static BigDecimal multiplyDecimalAB( BigDecimal a, double b)
	{
		BigDecimal ret = BigDecimal.valueOf(a.doubleValue());
		BigDecimal v = new BigDecimal( b);
		return ret.multiply(v);
	}

	public static BigDecimal divideAB( int a, int b)
	{
		BigDecimal v1 = new BigDecimal( a);
		BigDecimal v2 = new BigDecimal( b);
		return v1.divide( v2, 2, BigDecimal.ROUND_HALF_UP);
	}

	public static boolean isAGtB( BigDecimal a, BigDecimal b)
	{
		return a.compareTo( b) > 0;
	}


	public static String objToJsonStr( Object obj)
	{
		return JSON.toJSONString( obj);
	}

	public static JSONObject objToJson( Object obj)
	{
		return (JSONObject)JSON.toJSON( obj);
	}

	public static JSONObject jsonStrToJsonObj( String str)
	{
		return JSON.parseObject( str);
	}

	public static <T> List<T> getListFromJSON( JSONObject obj, String key, Class<T> type)
	{
		JSONArray arr = obj.getJSONArray(key);
		if( arr == null) return new ArrayList<T>();

		return arr.toJavaList( type);
	}

	public static <T> T jsonStrToObj( String str, Class<T> type)
	{
		return JSON.parseObject( str, type);
	}

	public static <T> List<T> jsonStrToList( String str, Class<T> type)
	{
		return JSON.parseArray( str, type);
	}
}
