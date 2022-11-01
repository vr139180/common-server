package com.cms.designer.coremodule;

import java.io.File;
import java.io.FileInputStream;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Properties;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import com.cms.designer.util.ConstValue;

/**
 * @author Administrator
 * 模块信息
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEModule
{
	//模块描述文件
	public final static String descfile ="module.xml";
	
	private String moduleID ="";
	private String moduleName ="";
	private String moduleClass ="";
	private String workDir ="";
	
	//目录位置
	private String rootPath ="";
	//用到的资源信息
	private Properties resource =new Properties();
	//系统菜单的动作实现
	private HashMap	ideAction =new HashMap();
	private HashSet singleActions =new HashSet();
	
	public OBEModule( File md) throws LoadModuleErrorException
	{
		rootPath =md.getPath();
		//验证正确性
		File ff =new File( md, descfile);
		if( !ff.exists())
			throw new LoadModuleErrorException();
		
		//初始化模块信息
		init( ff);
		
		if( !validateModule())
			throw new LoadModuleErrorException();
	}
	
	/**
	 * 获取资源
	 * @param id
	 * @return
	 */
	public String getResource( String id)
	{
		return resource.getProperty( id);
	}
	
	/**
	 * 初始化模块信息
	 * @param f
	 */
	private void init( File f)
	{
		SAXBuilder builder =new SAXBuilder();
		try
		{
			Document doc =builder.build( f);
			Element root =doc.getRootElement();
			analyModuleInfo( root);
			
			//分析系统菜单
			Element ideE =root.getChild( "ide.menu");
			analyIDEMenuAction( ideE);
			
			//加载资源
			loadResource();
		}
		catch( Exception e){
			e.printStackTrace();
		}
	}
	
	/**
	 * 加载资源
	 */
	private void loadResource()
	{
		try
		{
			String f2 ="resource_"+Locale.getDefault().getLanguage()+"_"+
				Locale.getDefault().getCountry()+".properties";
			File f =new File( rootPath, f2);
			FileInputStream fin =new FileInputStream( f);
			resource.load( fin);
			
			fin.close();
		}
		catch( Exception e1){
			e1.printStackTrace();
		}
	}
	
	/**
	 * 分析系统菜单重载情况
	 * @param e
	 */
	private void analyIDEMenuAction( Element e)
	{
		if( e == null) return;
		ClassLoader cl =this.getClass().getClassLoader();
		
		List ll =e.getChildren( "extension");
		for( int i =0; i < ll.size(); ++i)
		{
			Element ext =( Element)ll.get( i);
			String t =ext.getAttributeValue( "point");
			if( !ConstValue.containMenuType( t))
				continue;
			//保存action
			HashSet v =new HashSet();
			ideAction.put( t, v);
			
			List ll2 =ext.getChildren( "action");
			for( int j =0; j<ll2.size(); ++j)
			{
				Element act =( Element)ll2.get( j);
				String id =act.getAttributeValue( "id");
				if( !ConstValue.containSystemMenu( id))
					continue;
				String cls =act.getAttributeValue( "class");
				try
				{
					Class c =cl.loadClass( cls);
					v.add( id);
					singleActions.add( id);
				}
				catch( Exception ee){
					ee.printStackTrace();
				}
			}
		}
	}
	
	/**
	 * 判断是否包含动作
	 * @param id
	 * @return
	 */
	public boolean containAction( String id)
	{
		return singleActions.contains( id);
	}
	
	/**
	 * 分析模块描述信息
	 * @param r
	 */
	private void analyModuleInfo( Element r)
	{
		moduleID =r.getAttributeValue( "id");
		moduleName =r.getAttributeValue( "name");
		moduleClass =r.getAttributeValue( "class");
		workDir =r.getAttributeValue( "work-dir");
	}
	
	/**
	 * 验证模块的属性合法性
	 * @return
	 */
	private boolean validateModule()
	{
		if( moduleID == null || moduleID.equals( ""))
			return false;
		if( moduleName == null || moduleName.equals( ""))
			return false;
		if( moduleClass == null || moduleClass.equals( ""))
			return false;
		return true;
	}
	/**
	 * @return
	 */
	public String getModuleClass()
	{
		return moduleClass;
	}

	/**
	 * @return
	 */
	public String getModuleID()
	{
		return moduleID;
	}

	/**
	 * @return
	 */
	public String getModuleName()
	{
		return moduleName;
	}

	/**
	 * @return
	 */
	public String getWorkDir()
	{
		return workDir;
	}

	/**
	 * @return
	 */
	public String getRootPath()
	{
		return rootPath;
	}

}
