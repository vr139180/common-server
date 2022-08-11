package com.cms.designer.util;

import java.util.Map;

public class ExtendedAttributeTools
{

	private ExtendedAttributeTools()
	{
		// no op
	}

	public static Integer getInteger( Map extendedAttributes, String key)
	{
		String value = (String) extendedAttributes.get( key);
		if( value == null){ return null; }

		return new Integer( value);
	}

	public static String getString( Map extendedAttributes, String key)
	{
		String value = (String) extendedAttributes.get( key);
		if( value == null){ return null; }

		return new String( value);
	}

}
