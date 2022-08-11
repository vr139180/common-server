package com.cms.core.workflow.application;

import java.util.List;
import java.util.ArrayList;

import com.cms.core.AbstractElement;

/**
 * Abstract base implementation of the Application interface.
 */
public abstract class AbstractApplication extends AbstractElement implements Application
{
	private List formalParameters;

	/**
	 * Construct a new AbstractApplication.
	 * 
	 * @param id
	 *            The application id
	 * @param name
	 *            The application name
	 */

	public AbstractApplication( String id, String name)
	{
		super( id, name);

		this.formalParameters = new ArrayList();
	}

	/**
	 * Return a List of all FormalParameters for the application.
	 * 
	 * @return A List of FormalParameter objects
	 */

	public List getFormalParameters()
	{
		return formalParameters;
	}

}
