package com.cms.core.workflow.serializer;

import java.io.IOException;
import java.io.OutputStream;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.parser.XPDLNames;

/**
 * Standard interface for classes which can serialize a Package to an XPDL
 * document.
 */
public interface XPDLSerializer extends XPDLNames
{

	/**
	 * Serialize the package to the given output stream.
	 * 
	 * @param pkg
	 *            The Package
	 * @param out
	 *            The OutputStream
	 * @throws IOException
	 *             Any I/O exception
	 * @throws XPDLSerializerException
	 *             Any serializer Exception
	 */
	public void serialize( WorkflowPackage pkg, OutputStream out) throws IOException,
			XPDLSerializerException;

}
