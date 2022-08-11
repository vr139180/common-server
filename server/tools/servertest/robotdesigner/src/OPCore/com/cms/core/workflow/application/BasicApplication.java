package com.cms.core.workflow.application;

/**
 * Concrete implementation of the Application interface.
 */
public class BasicApplication extends AbstractApplication
{

	/**
	 * Construct a new application.
	 * 
	 * @param id
	 *            The application id
	 * @param name
	 *            The application name
	 */
	public BasicApplication( String id, String name)
	{
		super( id, name);
	}

}
