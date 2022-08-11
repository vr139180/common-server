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
 * ģ����Ϣ
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEModule
{
	//ģ�������ļ�
	public final static String descfile ="module.xml";
	
	private String moduleID ="";
	private String moduleName ="";
	private String moduleClass ="";
	private String workDir ="";
	
	//Ŀ¼λ��
	private String rootPath ="";
	//�õ�����Դ��Ϣ
	private Properties resource =new Properties();
	//ϵͳ�˵��Ķ���ʵ��
	private HashMap	ideAction =new HashMap();
	private HashSet singleActions =new HashSet();
	
	public OBEModule( File md) throws LoadModuleErrorException
	{
		rootPath =md.getPath();
		//��֤��ȷ��
		File ff =new File( md, descfile);
		if( !ff.exists())
			throw new LoadModuleErrorException();
		
		//��ʼ��ģ����Ϣ
		init( ff);
		
		if( !validateModule())
			throw new LoadModuleErrorException();
	}
	
	/**
	 * ��ȡ��Դ
	 * @param id
	 * @return
	 */
	public String getResource( String id)
	{
		return resource.getProperty( id);
	}
	
	/**
	 * ��ʼ��ģ����Ϣ
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
			
			//����ϵͳ�˵�
			Element ideE =root.getChild( "ide.menu");
			analyIDEMenuAction( ideE);
			
			//������Դ
			loadResource();
		}
		catch( Exception e){
			e.printStackTrace();
		}
	}
	
	/**
	 * ������Դ
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
	 * ����ϵͳ�˵��������
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
			//����action
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
	 * �ж��Ƿ��������
	 * @param id
	 * @return
	 */
	public boolean containAction( String id)
	{
		return singleActions.contains( id);
	}
	
	/**
	 * ����ģ��������Ϣ
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
	 * ��֤ģ������ԺϷ���
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
