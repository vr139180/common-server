package com.cms.core.workflow;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

import com.cms.core.*;
import com.cms.core.workflow.serializer.dom4j.Dom4JXPDLSerializer;

public class WorkflowPackage extends AbstractElement
{
	private List workflowProcesses;

	//package保存的文件
	private File file = null;

	/**
	 * @param id
	 * @param name
	 * @param packageHeader
	 *            @roseuid 3DEC73660163
	 */
	public WorkflowPackage( String id, String name)
	{
		super( id, name);

		this.workflowProcesses = new ArrayList();
	}

	/**
	 * 初始化包的初试信息
	 */
	public void initPackage()
	{
	}

	public WorkflowProcess getWorkflowProcess( String name)
	{	
		WorkflowProcess ret = null;
		Iterator f = workflowProcesses.iterator();
		while( f.hasNext())
		{
			ret = (WorkflowProcess) f.next();
			if( ret.getName().equals( name))
				break;
			else
				ret = null;
		}

		return ret;
	}

	/**
	 * 保存xpdl文件
	 */
	public void savePackageFile()
	{
		if( file == null)
			return;
		FileOutputStream out = null;
		try
		{
			out = new FileOutputStream( file);
			Dom4JXPDLSerializer serializer = new Dom4JXPDLSerializer();
			serializer.serialize( this, out);
			out.close();
			
			saveXmlFile();
		}
		catch( Exception e11)
		{
			e11.printStackTrace();
		}
	}
	
	public void saveXmlFile()
	{
		FileOutputStream out = null;
		try
		{
			File f2 =new File( file.getParent(), this.getName()+".xml");
			if( !f2.exists())
			{
				f2.createNewFile();
			}
			
			out = new FileOutputStream( f2);
			Dom4JXPDLSerializer serializer = new Dom4JXPDLSerializer();
			serializer.serialize2( this, out);
			out.close();
		}
		catch( Exception e11)
		{
			e11.printStackTrace();
		}
	}

	/**
	 * @return java.util.List @roseuid 3DEC73660311
	 */
	public List getWorkflowProcesses()
	{
		return workflowProcesses;
	}

	/**
	 * @throws java.lang.Throwable
	 *             @roseuid 3DF45EC00228
	 */
	protected void finalize() throws Throwable
	{
		super.finalize();
	}

	/**
	 * @param list
	 */
	public void setWorkflowProcesses( List list)
	{
		workflowProcesses = list;
	}

	/**
	 * @return
	 */
	public File getFile()
	{
		return file;
	}

	/**
	 * @param file
	 */
	public void setFile( File file)
	{
		this.file = file;
	}
	
}
