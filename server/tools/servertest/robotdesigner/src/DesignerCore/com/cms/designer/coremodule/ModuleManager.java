package com.cms.designer.coremodule;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Iterator;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * ��ʼ������ģ�飬���ҽ��й���
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class ModuleManager
{
	//ģ�龲̬����
	private HashMap modules =new HashMap();
	
	/**
	 * ע��һ��ģ��
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
	 * ��ȡ��Դ��Ϣ
	 * @param mid ģ��id
	 * @param id ��Դid
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
	 * ��ȡ����ģ����ϢOBEModuleStandard
	 * @return
	 */
	public Iterator getAllModules()
	{
		return modules.values().iterator();
	}
	
	/**
	 * ��ȡģ����Ϣ
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
