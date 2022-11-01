package com.cms.designer.workflow;

import java.io.File;
import java.io.FileInputStream;

import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.anthonyeden.lib.event.ErrorListener;
import com.anthonyeden.lib.event.ErrorListenerSupport;
import com.anthonyeden.lib.util.IOUtilities;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.parser.dom4j.Dom4JXPDLParser;
import com.cms.designer.swingui.OBEDesigner;

/**
 * A threaded class for loading workflow definitions.
 */
public class OpenWorkflowThread extends Thread
{

	private static final Log log = LogFactory.getLog( OpenWorkflowThread.class);

	private File file;

	private OBEDesigner parent;

	private ErrorListenerSupport errorListenerSupport;

	private String projectName;

	private String fileName;

	private String processName;

	public OpenWorkflowThread( OBEDesigner parent)
	{
		this.parent = parent;

		errorListenerSupport = new ErrorListenerSupport( this);
	}

	/**
	 * Load the workflow definition from the given file.
	 * 
	 * @param file
	 *            The file
	 * @throws Exception
	 */

	public void loadWorkflowDefinition( File file) throws Exception
	{
		this.file = file;
		start();
	}

	public void run()
	{
		FileInputStream in = null;
		WorkflowPackage workflowPackage = null;
		parent.setCursor( java.awt.Cursor.getPredefinedCursor( java.awt.Cursor.WAIT_CURSOR));
		try
		{
			log.info( "Parsing workflow definition");
			parent.setStatus( "Opening workflow from " + file.getAbsolutePath());
			in = new FileInputStream( file);
			Dom4JXPDLParser parser = new Dom4JXPDLParser();
			workflowPackage = parser.parse( in);
		}
		catch( Exception e)
		{
			errorListenerSupport.fireError( e);
			JOptionPane.showMessageDialog( null, "不能打开文件 " + file.getName(), "确定",
					JOptionPane.ERROR_MESSAGE);
		}
		finally
		{
			parent.setStatus( "");
			IOUtilities.close( in);
		}
		//		parent.getWorkSpaceManager().setCurrentElement(workflowPackage);
	}

	/**
	 * Add the specified error listener.
	 * 
	 * @param l
	 *            The ErrorListener
	 */

	public void addErrorListener( ErrorListener l)
	{
		errorListenerSupport.addErrorListener( l);
	}

	/**
	 * Remove the specified error listener.
	 * 
	 * @param l
	 *            The ErrorListener
	 */

	public void removeErrorListener( ErrorListener l)
	{
		errorListenerSupport.removeErrorListener( l);
	}

	/**
	 * @return
	 */
	public String getFileName()
	{
		return fileName;
	}

	/**
	 * @return
	 */
	public String getProcessName()
	{
		return processName;
	}

	/**
	 * @return
	 */
	public String getProjectName()
	{
		return projectName;
	}

	/**
	 * @param string
	 */
	public void setFileName( String string)
	{
		fileName = string;
	}

	/**
	 * @param string
	 */
	public void setProcessName( String string)
	{
		processName = string;
	}

	/**
	 * @param string
	 */
	public void setProjectName( String string)
	{
		projectName = string;
	}

}
