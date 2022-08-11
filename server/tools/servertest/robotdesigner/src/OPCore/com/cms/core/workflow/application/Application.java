package com.cms.core.workflow.application;

import java.util.List;

import com.cms.core.BasicElement;

/**
 * Base interface for defining an application which is accessible to a workflow
 * process. Applications are mapped to real applications by the runtime engine.
 */

public interface Application extends BasicElement
{

	/**
	 * Return a List of all FormalParameters for the application.
	 * 
	 * @return A List of FormalParameter objects
	 */
	public List getFormalParameters();

}
