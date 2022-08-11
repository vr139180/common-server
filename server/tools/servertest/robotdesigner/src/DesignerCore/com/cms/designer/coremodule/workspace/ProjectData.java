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
 * ��Ŀ
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: ProjectData.java,v 1.8 2004/08/17 07:23:37 york Exp $
 * $Id: ProjectData.java,v 1.8 2004/08/17 07:23:37 york Exp $
 */
public class ProjectData
{
	//��Ŀ�ļ�
	public static String SUFFIX = "project.wpr";
	
	public final static String PJ_ROOT ="project";
	public final static String PJ_MODULE ="module";
	public final static String PJ_ID ="moduleid";
	public final static String PJ_NAME ="name";
	
	//���
	private String projectID ="";
	//Ŀ¼��
	private String projectName ="";
	//Ŀ¼λ��
	private String path ="";
	private File file =null;
	
	//������ģ��
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
	 * ���ع�����Ϣ
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
	 * ������Ŀ�����Ϣ
	 */
	public void loadProject()
	{
		try
		{
			//����ģ������
			SAXBuilder builder =new SAXBuilder();
			Document doc =builder.build( file);
			Element root =doc.getRootElement();
			if( root != null)
			{
				//��ȡģ�����
				ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
				
				Iterator f =root.getChildren( PJ_MODULE).iterator();
				Element r =null;
				OBEModuleStandard ms =null;
				while( f.hasNext())
				{
					r =( Element)f.next();
					String id =r.getAttributeValue( PJ_ID);
					//����ģ����Ϣ
					ms =( OBEModuleStandard)mm.getModule( id);
					if( ms == null)	continue;
					OBEModuleData data =ms.loadModuleData( getProjectPath(), getProjectID());
					if( data == null)	continue;
					//����������Ϣ
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
	 * ����Ŀ¼
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

			//������Ŀ�����ļ�
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
	 * ���������ļ�
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
	 * ����һ��ģ��
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
		
		//�޸���Ŀ�����ļ�
		modifyDescriptionFile();
		
		return ret;
	}
	
	/**
	 * �޸Ĺ��������ļ�
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
	 * ��ȡ��Ŀ·��
	 * @return
	 */
	public String getProjectPath()
	{
		return path+File.separator+projectName;
	}
	
	/**
	 * ģ���Ƿ��Ѿ�����
	 * @param mid
	 * @return
	 */
	public boolean containModule( String mid)
	{
		return moduleInfo.containsKey( mid);
	}
	
	/**
	 * ��ȡ�ƶ�ģ���uuid
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
	 * ������Ŀ����
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

			//��ȡ��ص�ģ������
			//������ص�ģ��
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
