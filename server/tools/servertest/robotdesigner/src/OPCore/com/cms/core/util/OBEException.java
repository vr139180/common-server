package com.cms.core.util;

/**
 * Base exception for all exceptions in the Open Business Engine.
 */
public class OBEException extends Exception
{

	/** Construct a new OBEException. */

	public OBEException()
	{
		super();
	}

	/**
	 * Construct a new OBEException with the given message.
	 * 
	 * @param message
	 *            The message
	 */

	public OBEException( String message)
	{
		super( message);
	}

	/**
	 * Construct a new OBEException with the given nested error.
	 * 
	 * @param t
	 *            The nested error
	 */

	public OBEException( Throwable t)
	{
		super( t.getMessage());
		this.nestedException = t;
	}

	/**
	 * Construct a new OBEException with the given messager and nested error.
	 * 
	 * @param message
	 *            The message
	 * @param t
	 *            The nested error
	 */

	public OBEException( String message, Throwable t)
	{
		super( message);
		this.nestedException = t;
	}

	/**
	 * Get the nested error.
	 * 
	 * @return The nested error
	 */

	public Throwable getNestedException()
	{
		return nestedException;
	}

	protected Throwable nestedException;

}
