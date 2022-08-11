package com.cms.core.workflow.activity;

import java.util.HashMap;
import java.util.Iterator;

public final class ImplementationType
{
	/** Integer representing the NO implementation type. */
	public static final int NO_TYPE = 1;

	/** Integer representing the TOOL implementation type. */
	public static final int TOOL_TYPE = 2;

	/** ImplementationType which represents an empty implementation. */
	public static final ImplementationType NO = new ImplementationType( NO_TYPE);

	/** ImplementationType which represents a tool implementation. */
	public static final ImplementationType TOOL = new ImplementationType( TOOL_TYPE);

	/** An array of all implementation types. */
	public static final ImplementationType[] types = {NO, TOOL};

	private static HashMap stringMap = new HashMap();

	static
	{
		stringMap.put( NO, "No");
		stringMap.put( TOOL, "Tool");
	}

	private int value;

	/**
	 * Construct a new ImplementationType instance.
	 * 
	 * @param value
	 *            The value
	 */

	private ImplementationType( int value)
	{
		this.value = value;
	}

	/**
	 * Get the value. This value can be used in switch statements in
	 * conjunction with the xxx_VALUE contants defined in this class.
	 * 
	 * @return An int value
	 */

	public int getValue()
	{
		return value;
	}

	/**
	 * Return a String representation of this class.
	 * 
	 * @return The String representation
	 */

	public String toString()
	{
		return (String) stringMap.get( this);
	}

	/**
	 * Convert the specified ImplementationType to a String.
	 * 
	 * @param type
	 *            The ImplementationType
	 * @return The String
	 */

	public static String typeToString( ImplementationType type)
	{
		return (String) stringMap.get( type);
	}

	/**
	 * Convert the specified String to an ImplementationType object. If there
	 * no matching ImplementationType for the given String then this method
	 * returns null.
	 * 
	 * @param mode
	 *            The String
	 * @return The ImplementationType object
	 */

	public static ImplementationType stringToType( String type)
	{
		Iterator keys = stringMap.keySet().iterator();
		while( keys.hasNext())
		{
			ImplementationType key = (ImplementationType) keys.next();
			String stringValue = (String) stringMap.get( key);
			if( stringValue.equals( type)){ return key; }
		}
		return null;
	}

}
