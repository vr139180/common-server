package com.cms.designer;

import java.util.HashMap;
import java.util.Map;

/**
 * @author Administrator
 * 
 */
public class UserModeType
{
	private static boolean isExp()
	{
		String mode =System.getProperty( "user.mode", "exp");
		return mode.equals( "exp");
	}
	
	private static boolean isEbseries()
	{
		String mode =System.getProperty( "user.mode", "exp");
		return mode.equals( "ebseries");
	}
	
	public static Map getDutyInfo()
	{
		Map ret =new HashMap();
		
		ret.put( "", "");
		
	
		return ret;
	}
}
