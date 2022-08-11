package com.cms.designer.util;

import java.util.HashMap;
import java.util.Iterator;

public final class EditMode
{
	public static final int VIEW_TYPE = 1;

	public static final int EDIT_TYPE = 2;

	public static final int CONNECT_TYPE = 3;

	public static final EditMode VIEW = new EditMode( VIEW_TYPE);

	public static final EditMode EDIT = new EditMode( EDIT_TYPE);

	public static final EditMode CONNECT = new EditMode( CONNECT_TYPE);

	private static HashMap stringMap = new HashMap();

	static
	{
		stringMap.put( VIEW, "View");
		stringMap.put( EDIT, "Edit");
		stringMap.put( CONNECT, "Connect");
	}

	private int value;

	private EditMode( int value)
	{
		this.value = value;
	}

	public int getValue()
	{
		return value;
	}

	public String toString()
	{
		return (String) stringMap.get( this);
	}

	public static String typeToString( EditMode type)
	{
		return (String) stringMap.get( type);
	}

	public static EditMode stringToType( String type)
	{
		Iterator keys = stringMap.keySet().iterator();
		while( keys.hasNext())
		{
			EditMode key = (EditMode) keys.next();
			String stringValue = (String) stringMap.get( key);
			if( stringValue.equals( type)){ return key; }
		}
		return null;
	}

}
