package com.cms.core;

import java.util.Map;
import java.util.HashMap;

/**
 * Abstract implementation of the WFElement interface which implements base
 * funtionality used by all WFElement implementations.
 */
public abstract class AbstractElement implements BasicElement
{
	private String id;

	private String name;

	private Map extendedAttributes;

	/**
	 * Construct a new AbstractWFElement.
	 * 
	 * @param id
	 *            The element ID
	 * @param name
	 *            The element name
	 */

	protected AbstractElement( String id, String name)
	{
		setId( id);
		setName( name);

		extendedAttributes = new HashMap();
	}

	/**
	 * Get the workflow element's ID.
	 * 
	 * @return The ID
	 */

	public String getId()
	{
		return id;
	}

	/**
	 * Set the element's unique ID.
	 * 
	 * @param id
	 *            The new unique ID
	 */

	public void setId( String id)
	{
		if( id == null){ throw new IllegalArgumentException( "ID cannot be null"); }
		this.id = id;
	}

	/**
	 * Get the workflow element's name.
	 * 
	 * @return The name
	 */

	public String getName()
	{
		return name;
	}

	/**
	 * Set the workflow element's name.
	 * 
	 * @param name
	 *            The new name
	 */

	public void setName( String name)
	{
		this.name = name;
	}

	/**
	 * Get a Map of extended attributes.
	 * 
	 * @return A Map of extended attributes
	 */

	public Map getExtendedAttributes()
	{
		return extendedAttributes;
	}
}
