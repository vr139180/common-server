package com.cms.core.workflow.activity;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Iterator;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: AutomationMode.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public final class AutomationMode implements Serializable
{
	/** Integer representing the AUTOMATIC type. */
	public static final int AUTOMATIC_TYPE = 1;

	/** AutomationMode representing an automatic start or completion. */
	public static final AutomationMode AUTOMATIC = new AutomationMode( AUTOMATIC_TYPE);

	/** An array of all automation modes. */
	public static final AutomationMode[] types = {AUTOMATIC};

	private static HashMap stringMap = new HashMap();

	static
	{
		stringMap.put( AUTOMATIC, "Automatic");
	}

	private int value;

	/**
	 * Construct a new AutomationMode instance.
	 * 
	 * @param value
	 *            The value
	 */

	private AutomationMode( int value)
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
	 * Convert the specified AutomationMode to a String.
	 * 
	 * @param mode
	 *            The AutomationMode
	 * @return The String
	 */

	public static String modeToString( AutomationMode mode)
	{
		return (String) stringMap.get( mode);
	}

	/**
	 * Convert the specified String to an AutomationMode object. If there no
	 * matching AutomationMode for the given String then this method returns
	 * null.
	 * 
	 * @param mode
	 *            The String
	 * @return The AutomationMode object
	 */

	public static AutomationMode stringToMode( String mode)
	{
		Iterator keys = stringMap.keySet().iterator();
		while( keys.hasNext())
		{
			AutomationMode automationMode = (AutomationMode) keys.next();
			String stringValue = (String) stringMap.get( automationMode);
			if( stringValue.equals( mode)){ return automationMode; }
		}
		return null;
	}

}
