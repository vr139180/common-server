package com.cms.core.util;

import java.util.ResourceBundle;

/**
 * @author Administrator
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class ResourceUtil
{
	private static ResourceBundle res;

	public static void init( String resourcePath)
	{
		try
		{
			res = ResourceBundle.getBundle( resourcePath);
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}

	public static String getRS( String key)
	{
		return res.getString( key);
	}
}
