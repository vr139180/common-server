package com.cms.designer.coremodule;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.cms.designer.coremodule.workspace.OBEModuleData;
import com.cms.designer.coremodule.workspace.ProjectData;

/**
 * @author Administrator 项目数据管理 To change the template for this generated type
 *         comment go to Window - Preferences - Java - Code Generation - Code
 *         and Comments $Id: ProjectManager.java,v 1.1.4.2 2004/06/08 02:59:28
 *         gaj Exp $
 */
public class ProjectManager
{
	//编号计数
	private long projectID = 0;

	private HashMap projects = new HashMap();

	/**
	 * 增加一个新的项目
	 * 
	 * @param pe
	 */
	public synchronized OBEModuleData newProject( ProjectData pe)
	{
		++projectID;
		pe.setProjectID( Long.toString( projectID));
		projects.put( pe.getProjectID(), pe);
		
		
		//auto init workflow module
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =mm.getFirstModule();
		
		String moduleID =ms.getOBEModule().getModuleID();
		OBEModuleData md =ms.newModuleData( pe.getProjectID(), moduleID);
		//在项目数据中注册
		pe.appendModule( moduleID, md.getUuid());
		md.mkModuleDir( pe.getProjectPath());
		
		return md;
	}

	/**
	 * 根据项目id获取项目描述
	 * 
	 * @param pid
	 * @return
	 */
	public ProjectData getOneProject( String pid)
	{
		return (ProjectData) projects.get( pid);
	}

	/**
	 * 获取所有的项目信息 projectdata对象
	 * 
	 * @return
	 */
	public List getAllProjects()
	{
		return new ArrayList( projects.values());
	}

	public ProjectData getOneProjectByName( String name)
	{
		ProjectData pd = null;
		List pdList = getAllProjects();
		String pdName = null;

		int i = 0;
		for( i = 0; i < pdList.size(); i++)
		{
			pd = (ProjectData) pdList.get( i);
			pdName = pd.getProjectName();
			if( pdName.equals( name))
				break;
			else
				pd = null;
		}

		return pd;
	}

	public void removeOneProject( String pid)
	{
		//remove its physical directory
		ProjectManager pm = OBEModuleManager.getInstance().getProjectManager();
		ProjectData pd = pm.getOneProject( pid);
		File projectPath = new File( pd.getProjectPath());
		deleteFile( projectPath);

		//remove the registed informations
		projects.remove( pid);

		return;
	}

	public static void deleteFile( File f)
	{
		if( f.isDirectory())
		{
			File d[] = f.listFiles();
			for( int i = 0; i < d.length; ++i)
				deleteFile( d[i]);
		}
		f.delete();
	}
	


}
