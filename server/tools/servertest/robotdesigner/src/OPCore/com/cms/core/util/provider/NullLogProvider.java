package com.cms.core.util.provider;

import com.cms.core.util.log.LogProvider;
import com.cms.core.util.log.Logger;

/**
 * A LogProvider and Logger which does not log messages, it just discards them.
 */
public class NullLogProvider implements LogProvider, Logger
{

	/**
	 * Get the Logger instance.
	 * 
	 * @param id
	 *            The id of the logger
	 */
	public Logger getLogger( String id)
	{
		return this;
	}

	public void fatal( String message)
	{
	}

	public void error( String message)
	{
	}

	public void warn( String message)
	{
	}

	public void info( String message)
	{
	}

	public void debug( String message)
	{
	}

}
