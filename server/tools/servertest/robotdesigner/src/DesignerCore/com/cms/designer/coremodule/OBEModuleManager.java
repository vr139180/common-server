package com.cms.designer.coremodule;

import java.io.File;
import java.net.URLClassLoader;
import java.util.HashMap;
import java.util.Iterator;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * ģ�����
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEModuleManager
{
	//ģ��·��
	public final static String modulepath =System.getProperty("ide.home")
				+File.separator+"modules";
	
	private static OBEModuleManager instance =null;
	private boolean moduleInitialized =false;
	//����ģ����ص�classloader
	private URLClassLoader classLoader =null;
	
	//���ģ����Ϣ
	private HashMap modules =new HashMap();
	//ģ�����
	private ModuleManager moduleManager =new ModuleManager();
	//��Ŀ���ݹ���
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
	 * ��ʼ��ģ��
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
	 * �������е�ģ��
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
	 * ��ȡģ�������
	 * @return
	 */
	public ModuleManager getModuleManager()
	{
		return moduleManager;
	}

	/**
	 * ��ȡ��Ŀ���ݹ�����
	 * @return
	 */
	public ProjectManager getProjectManager()
	{
		return projectManager;
	}
	

}
