package com.cms.core.workflow.parser;

import com.cms.core.util.OBEException;

/**
 * Exception which is thrown when there is an error parsing an XPDL document.
 */
public class XPDLParserException extends OBEException
{

	/** Construct a new XPDLParserException. */

	public XPDLParserException()
	{
		super();
	}

	/**
	 * Construct a new XPDLParserException with the specified message.
	 * 
	 * @param message
	 *            The error message
	 */

	public XPDLParserException( String message)
	{
		super( message);
	}

	/**
	 * Construct a new XPDLParserException with the specified nested error.
	 * 
	 * @param t
	 *            The nested error
	 */

	public XPDLParserException( Throwable t)
	{
		super( t);
	}

	/**
	 * Construct a new XPDLParserException with the specified error message and
	 * nested exception.
	 * 
	 * @param message
	 *            The error message
	 * @param t
	 *            The nested error
	 */

	public XPDLParserException( String message, Throwable t)
	{
		super( message, t);
	}

}
