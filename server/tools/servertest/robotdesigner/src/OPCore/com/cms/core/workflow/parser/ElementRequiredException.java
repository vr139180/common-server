package com.cms.core.workflow.parser;

/**
 * Exception which is thrown when an element is required and is missing.
 */
public class ElementRequiredException extends XPDLParserException
{
	private String elementName;

	/**
	 * Construct a new ElementRequiredException.
	 * 
	 * @param elementName
	 *            The element name
	 */

	public ElementRequiredException( String elementName)
	{
		this( elementName, null);
	}

	/**
	 * Construct a new ElementRequiredException.
	 * 
	 * @param elementName
	 *            The element name
	 * @param message
	 *            The error message
	 */

	public ElementRequiredException( String elementName, String message)
	{
		super( message);
		this.elementName = elementName;
	}

	/**
	 * Return the name of the missing element.
	 * 
	 * @return The missing element name
	 */

	public String getElementName()
	{
		return elementName;
	}

}
