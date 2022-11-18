package com.cms.commons.redis;

import com.alibaba.fastjson.JSON;

import java.nio.charset.Charset;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
public class JsonSerializer
{
	public static final String UTF_8 = "UTF-8";
	/**
	 * @param obj
	 * @param <T>
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public static <T> byte[] serialize(T obj) {
		return JSON.toJSONString(obj).getBytes(Charset.forName(UTF_8));
	}
	/**
	 * @param data
	 * @param clazz
	 * @param <T>
	 * @return
	 */
	public static <T> T deserialize(byte[] data, Class<T> clazz) {
		return JSON.parseObject(data, clazz);
	}
}
