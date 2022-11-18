package com.cms.commons.utils;

import java.util.UUID;

/**
 * Created by Administrator on 2020/8/18.
 */
public final class GUIDUtils
{
	public static String genGUID()
	{
		UUID id = UUID.randomUUID();
		return id.toString().toLowerCase();
	}

	public static String genGUIDNoDelimiter()
	{
		String uid = genGUID();
		return uid.replaceAll( "-", "");
	}
}

