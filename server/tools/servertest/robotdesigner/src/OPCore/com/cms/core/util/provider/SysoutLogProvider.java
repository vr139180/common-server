package com.cms.core.util.provider;

import java.util.HashMap;

import com.cms.core.util.log.LogProvider;
import com.cms.core.util.log.Logger;

/**
 * Implementation of the LogProvider interface which creates loggers which
 * write messages to the standard output stream.
 */
public class SysoutLogProvider implements LogProvider
{
	private HashMap loggers = new HashMap();

	/**
	 * Get a Logger instance for the given id.
	 * 
	 * @param id
	 *            The logger ID
	 * @return The Logger instance
	 */
	public synchronized Logger getLogger( String id)
	{
		Logger logger = (Logger) loggers.get( id);
		if( logger == null)
		{
			logger = new LoggerImpl( id);
			loggers.put( id, logger);
		}
		return logger;
	}

	/**
	 * Implementation of the Logger interface which prints messages to the
	 * standard output stream.
	 * 
	 * @author Anthony Eden
	 */
	class LoggerImpl implements Logger
	{

		public LoggerImpl( String id)
		{
			this.id = id;
		}

		public void fatal( String message)
		{
			printMessage( "FATAL", message);
		}

		public void error( String message)
		{
			printMessage( "ERROR", message);
		}

		public void warn( String message)
		{
			printMessage( "WARN", message);
		}

		public void info( String message)
		{
			printMessage( "INFO", message);
		}

		/**
		 * Print a message at DEBUG level.
		 * 
		 * @param message
		 *            The message
		 */
		public void debug( String message)
		{
			printMessage( "DEBUG", message);
		}

		/**
		 * Pring a message at the specified level.
		 * 
		 * @param level
		 *            The log level
		 * @param message
		 *            The message
		 */
		private void printMessage( String level, String message)
		{
			StringBuffer buffer = new StringBuffer();
			buffer.append( id);
			buffer.append( " [");
			buffer.append( level);
			buffer.append( "]: ");
			buffer.append( message);
		}

		private String id;

	}

}
