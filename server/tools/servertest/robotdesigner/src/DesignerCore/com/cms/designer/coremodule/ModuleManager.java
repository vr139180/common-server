package com.cms.designer.coremodule;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Iterator;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * 初始化各个模块，并且进行管理
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class ModuleManager
{
	//模块静态数据
	private HashMap modules =new HashMap();
	
	/**
	 * 注册一个模块
	 * @param m
	 * @return
	 */
	public boolean registerModule( OBEModule m, OBEDesigner designer)
	{
		boolean ret =false;
		ClassLoader cl =this.getClass().getClassLoader();
		try
		{
			Class c =cl.loadClass( m.getModuleClass());
			Constructor co =c.getConstructor( new Class[]{ OBEModule.class, OBEDesigner.class});
			Object obj =co.newInstance( new Object[]{ m, designer});
			if( obj instanceof OBEModuleStandard)
			{
				modules.put( m.getModuleID(), obj);
				ret =true;
			}
		}
		catch( Exception e1){
			e1.printStackTrace();
		}
		
		return ret;
	}
	
	/**
	 * 获取资源信息
	 * @param mid 模块id
	 * @param id 资源id
	 * @return
	 */
	public String getResource( String mid, String id)
	{
		String ret ="";
		OBEModuleStandard ms =( OBEModuleStandard)modules.get( mid);
		if( ms != null)
			ret =ms.getOBEModule().getResource( id);
		
		return ret;
	}
	
	/**
	 * 获取所有模块信息OBEModuleStandard
	 * @return
	 */
	public Iterator getAllModules()
	{
		return modules.values().iterator();
	}
	
	/**
	 * 获取模块信息
	 * @param mid
	 * @return
	 */
	public OBEModuleStandard getModule( String mid)
	{
		return ( OBEModuleStandard)modules.get( mid);
	}
	
	public OBEModuleStandard getFirstModule()
	{
		return (OBEModuleStandard)getAllModules().next();
	}
}
