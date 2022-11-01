package com.cms.designer.coremodule.workspace;

import java.io.File;
import java.io.FileOutputStream;
import java.util.HashMap;
import java.util.Iterator;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;

import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.swingui.OBETree;
import com.cms.designer.swingui.OBETreeNode;

/**
 * @author Administrator
 * 项目
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: ProjectData.java,v 1.8 2004/08/17 07:23:37 york Exp $
 * $Id: ProjectData.java,v 1.8 2004/08/17 07:23:37 york Exp $
 */
public class ProjectData
{
	//项目文件
	public static String SUFFIX = "project.wpr";
	
	public final static String PJ_ROOT ="project";
	public final static String PJ_MODULE ="module";
	public final static String PJ_ID ="moduleid";
	public final static String PJ_NAME ="name";
	
	//编号
	private String projectID ="";
	//目录名
	private String projectName ="";
	//目录位置
	private String path ="";
	private File file =null;
	
	//包含的模块
	private HashMap moduleInfo =new HashMap();

	public HashMap getmodules(){
		return moduleInfo;
	}
	
	public ProjectData( String projectName, String path)
	{
		this.projectName =projectName;
		this.path =path;
		this.createPath();
	}
	
	private ProjectData()
	{
	}
	
	/**
	 * 加载工程信息
	 * @param projectName
	 * @param path
	 * @return
	 */
	public static ProjectData loadProject( String projectName, String path)
	{
		ProjectData pd =new ProjectData();
		pd.projectName =projectName;
		pd.path =path;
		
		File f1 =new File( path, projectName);
		try
		{
			pd.file =new File( f1, SUFFIX);
			if( !pd.file.exists())
			{
				pd.file.createNewFile();
				pd.createDescriptionFile( pd.file);
			}
		}
		catch( Exception ioe)
		{
			pd.file =null;
			ioe.printStackTrace();
		}
		
		return pd;
	}

	/**
	 * 加载项目相关信息
	 */
	public void loadProject()
	{
		try
		{
			//加载模块数据
			SAXBuilder builder =new SAXBuilder();
			Document doc =builder.build( file);
			Element root =doc.getRootElement();
			if( root != null)
			{
				//获取模块管理
				ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
				
				Iterator f =root.getChildren( PJ_MODULE).iterator();
				Element r =null;
				OBEModuleStandard ms =null;
				while( f.hasNext())
				{
					r =( Element)f.next();
					String id =r.getAttributeValue( PJ_ID);
					//查找模块信息
					ms =( OBEModuleStandard)mm.getModule( id);
					if( ms == null)	continue;
					OBEModuleData data =ms.loadModuleData( getProjectPath(), getProjectID());
					if( data == null)	continue;
					//建立关联信息
					moduleInfo.put( id, data.getUuid());
				}
			}
		}
		catch( Exception ioe){
			file =null;
			ioe.printStackTrace();
		}
	}
	
	public String getDefaultModuleKey(){
		if( moduleInfo.keySet().size() == 0)
			return "";
		return (String)moduleInfo.keySet().iterator().next();
	}

	/**
	 * 创建目录
	 */
	protected void createPath()
	{
		try
		{
			String packagePath =path+File.separator+projectName;
			File directory = new File( path);
			if (!directory.exists())
				directory.mkdir();
			directory =new File(packagePath); 
			if (!directory.exists())
				directory.mkdir();

			//创建项目描述文件
			file = new File( packagePath + File.separator + SUFFIX);
			if (!file.exists())
				file.createNewFile();
			createDescriptionFile( file);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * 创建描述文件
	 * @param f
	 */
	private void createDescriptionFile( File f)
	{
		try
		{
			Document doc =new Document();
			Element root =new Element( PJ_ROOT);
			
			doc =doc.setRootElement( root);
			FileOutputStream fout =new FileOutputStream( file);
			
			XMLOutputter out =new XMLOutputter();
			out.setEncoding( "UTF-8");
			out.setIndent( "\t");
			out.setNewlines( true);
			out.output( doc, fout);
			
			fout.close();
		}
		catch( Exception e){
		}
	}
	
	/**
	 * 增加一个模块
	 * @param mid
	 * @param uuid
	 * @return
	 */
	public boolean appendModule( String mid, String uuid)
	{
		boolean ret =true;
		if( moduleInfo.containsKey( mid))
			return false;
		moduleInfo.put( mid, uuid);
		
		//修改项目描述文件
		modifyDescriptionFile();
		
		return ret;
	}
	
	/**
	 * 修改工程描述文件
	 */
	private void modifyDescriptionFile()
	{
		if( file == null)	return;
		try
		{
			Element root =new Element( PJ_ROOT);
			Iterator f =moduleInfo.keySet().iterator();
			while( f.hasNext())
			{
				String mid =( String)f.next();
				Element e =new Element( PJ_MODULE);
				e =e.setAttribute( PJ_ID, mid);
				root =root.addContent( e);
			}
			Document doc =new Document();
			doc =doc.setRootElement( root);
			
			FileOutputStream fout =new FileOutputStream( file);
			XMLOutputter out =new XMLOutputter();
			out.setEncoding( "UTF-8");
			out.setIndent( "\t");
			out.setNewlines( true);
			out.output( doc, fout);
			
			fout.close();
		}
		catch( Exception e){
		}
	}
	
	/**
	 * 获取项目路径
	 * @return
	 */
	public String getProjectPath()
	{
		return path+File.separator+projectName;
	}
	
	/**
	 * 模块是否已经加载
	 * @param mid
	 * @return
	 */
	public boolean containModule( String mid)
	{
		return moduleInfo.containsKey( mid);
	}
	
	/**
	 * 获取制定模块的uuid
	 * @param mid
	 * @return
	 */
	public String getModuleUUID( String mid)
	{
		return ( String)moduleInfo.get( mid);
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

	/**
	 * @return
	 */
	public String getProjectName()
	{
		return projectName;
	}

	/**
	 * @param projectName
	 */
	public void setProjectName(String projectName)
	{
		this.projectName = projectName;
	}

	public String toString()
	{
		return projectName+":"+projectID;
	}
	
	/**
	 * @return
	 */
	public String getPath()
	{
		return path;
	}

	/**
	 * 属性项目数据
	 * @param tree
	 * @param parent
	 */
	public void refrushWorkspaceTree( OBETree tree, OBETreeNode parent)
	{
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		ElementKey ek =new ElementKey();
		ek.setProjectID( getProjectID());
		Iterator f =moduleInfo.keySet().iterator();
		while( f.hasNext())
		{
			String id =( String)f.next();
			String uuid =( String)moduleInfo.get( id);
			if( uuid == null)	continue;

			//获取相关的模块数据
			//查找相关的模块
			OBEModuleStandard ms =mm.getModule( id);
			if( ms == null)	continue;
			OBEModuleData data =ms.getModuleData( uuid);
			if( data == null)	continue;
			
			data.refrushWorkspaceTree( tree, parent);
		}
	}
	
	public void removeOneProject(String projectId)
	{
		this.moduleInfo.remove(projectId);
		return;
	}
	
}
