package com.cms.core.workflow.parser;

import java.io.InputStream;
import java.io.IOException;

import com.cms.core.workflow.WorkflowPackage;

/**
 * Standard interface for an XPDL parser.
 */
public interface XPDLParser extends XPDLNames
{

	/**
	 * Parse the given InputStream into a Package object.
	 * 
	 * @param in
	 *            The InputStream
	 * @throws IOException
	 *             Any I/O Exception
	 * @throws XPDLParserException
	 *             Any parser exception
	 */
	public WorkflowPackage parse( InputStream in) throws IOException, XPDLParserException;

}
