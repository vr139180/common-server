package com.cms.core.workflow.serializer;

import com.cms.core.util.OBEException;

/**
 * Exception which is thrown when there is an error serializing to an XPDL
 * document.
 */
public class XPDLSerializerException extends OBEException
{

	/** Construct a new XPDLSerializerException. */

	public XPDLSerializerException()
	{
		super();
	}

	/**
	 * Construct a new XPDLSerializerException with the given error message.
	 * 
	 * @param message
	 *            The error message
	 */

	public XPDLSerializerException( String message)
	{
		super( message);
	}

	/**
	 * Construct a new XPDLSerializerException with the given nested error.
	 * 
	 * @param t
	 *            The nested error
	 */

	public XPDLSerializerException( Throwable t)
	{
		super( t);
	}

	/**
	 * Construct a new XPDLSerializerException with the given error message and
	 * nested error.
	 * 
	 * @param message
	 *            The error message
	 * @param t
	 *            The error
	 */

	public XPDLSerializerException( String message, Throwable t)
	{
		super( message, t);
	}

}
