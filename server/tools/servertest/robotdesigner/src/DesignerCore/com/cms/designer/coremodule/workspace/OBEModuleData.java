package com.cms.designer.coremodule.workspace;

import java.io.File;

import com.cms.designer.swingui.OBETree;
import com.cms.designer.swingui.OBETreeNode;

/**
 * @author Administrator
 * 保存各个模块的数据信息
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEModuleData.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public abstract class OBEModuleData
{
	private String moduleID =null;
	private String projectID =null;
	private String uuid =null;
	
	public OBEModuleData( String moduleID, String projectID)
	{
		super();
		this.moduleID =moduleID;
		this.projectID =projectID;
	}
	
	//创建工作目录
	public abstract void mkModuleDir( String parent);
	//刷新模块数据显示
	public abstract void refrushWorkspaceTree( OBETree tree, OBETreeNode parent);
	
	public abstract void defaultModuleInit( OBETree tree, OBETreeNode parent);
	
	/**
	 * @return
	 */
	public String getUuid()
	{
		return uuid;
	}

	/**
	 * @param uuid
	 */
	public void setUuid(String uuid)
	{
		this.uuid = uuid;
	}

	/**
	 * @return
	 */
	public String getModuleID()
	{
		return moduleID;
	}

	/**
	 * @param moduleID
	 */
	public void setModuleID(String moduleID)
	{
		this.moduleID = moduleID;
	}

	/**
	 * @return
	 */
	public String getProjectID()
	{
		return projectID;
	}

	/**
	 * @param projectID
	 */
	public void setProjectID(String projectID)
	{
		this.projectID = projectID;
	}

	public void deleteFile( File f)
	{
		if( f.isDirectory())
		{
			File d[] =f.listFiles();
			for( int i =0; i<d.length; ++i)
				deleteFile( d[i]);
		}
		f.delete();
	}
}
