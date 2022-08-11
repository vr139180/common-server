package com.cms.core;

import java.util.Map;

public interface BasicElement
{

	/**
	 * Get the element's unique ID.
	 * 
	 * @return The unique ID
	 */
	public String getId();

	/**
	 * Set the element's unique ID.
	 * 
	 * @param id
	 *            The new unique ID
	 */
	public void setId( String id);

	/**
	 * Get an identifying name for the element.
	 * 
	 * @return The name
	 */
	public String getName();

	/**
	 * Set the identifying name for the element.
	 * 
	 * @param name
	 *            The new name
	 */
	public void setName( String name);


	/**
	 * Get a map of extended attributes.
	 * 
	 * @return A map of extended attributes
	 */
	public Map getExtendedAttributes();

}
