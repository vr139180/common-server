package com.cms.core.util;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Iterator;

public final class InstantiationType implements Serializable
{

	private InstantiationType( int value)
	{
		this.value = value;
	}

	public int getValue()
	{
		return value;
	}

	public String toString()
	{
		return typeToString( this);
	}

	public static String typeToString( InstantiationType type)
	{
		return (String) stringMap.get( type);
	}

	public static InstantiationType stringToType( String type)
	{
		Iterator keys = stringMap.keySet().iterator();
		while( keys.hasNext())
		{
			InstantiationType key = (InstantiationType) keys.next();
			String stringValue = (String) stringMap.get( key);
			if( stringValue.equals( type)){ return key; }
		}
		return null;
	}

	public static final int ONCE_TYPE = 1;

	public static final int MULTIPLE_TYPE = 2;

	public static final InstantiationType ONCE = new InstantiationType( ONCE_TYPE);

	public static final InstantiationType MULTIPLE = new InstantiationType( MULTIPLE_TYPE);

	private static HashMap stringMap = new HashMap();

	static
	{
		stringMap.put( ONCE, "ONCE");
		stringMap.put( MULTIPLE, "MULTIPLE");
	}

	private int value;

}
