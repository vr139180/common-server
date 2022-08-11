package com.cms.designer.coremodule;

import java.io.File;
import java.net.URLClassLoader;
import java.util.HashMap;
import java.util.Iterator;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * 模块管理
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEModuleManager
{
	//模块路径
	public final static String modulepath =System.getProperty("ide.home")
				+File.separator+"modules";
	
	private static OBEModuleManager instance =null;
	private boolean moduleInitialized =false;
	//用于模块加载的classloader
	private URLClassLoader classLoader =null;
	
	//存放模块信息
	private HashMap modules =new HashMap();
	//模块管理
	private ModuleManager moduleManager =new ModuleManager();
	//项目数据管理
	private ProjectManager projectManager =new ProjectManager();
	
	private OBEModuleManager(){
	}
	
	public static synchronized OBEModuleManager getInstance()
	{
		if( instance == null)
		{
			instance =new OBEModuleManager();
			instance.loadModules();
		}
		
		return instance;
	}
	
	/**
	 * 初始化模块
	 * @param designer
	 */
	public synchronized void initModules( OBEDesigner designer)
	{
		if( moduleInitialized)
			return;

		Iterator it =modules.values().iterator();
		OBEModule md =null;
		while( it.hasNext())
		{
			md =( OBEModule)it.next();
			moduleManager.registerModule( md, designer);
		}
	}
	
	/**
	 * 加载所有的模块
	 */
	private void loadModules()
	{
		File rd =new File( modulepath);
		if( !rd.exists() || !rd.isDirectory())
			return;
		File[] mds =rd.listFiles();
		if( mds.length == 0)
			return;
		for( int i =0; i < mds.length; ++i)
		{
			if( !mds[i].isDirectory())
				continue;
			try
			{
				OBEModule module =new OBEModule( mds[i]);
				modules.put( module.getModuleID(), module);
			}
			catch( LoadModuleErrorException lme){
				lme.printStackTrace();
			}
		}
	}
	
	/**
	 * @return
	 */
	public ClassLoader getClassLoader()
	{
		return classLoader;
	}

	/**
	 * 获取模块管理器
	 * @return
	 */
	public ModuleManager getModuleManager()
	{
		return moduleManager;
	}

	/**
	 * 获取项目数据管理器
	 * @return
	 */
	public ProjectManager getProjectManager()
	{
		return projectManager;
	}
	

}
