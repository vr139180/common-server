package com.cms.designer.manager;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Iterator;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;

import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETree;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class WorkSpaceManager implements PropertyNames
{
	public static String CONFIG = "config.properties";
	public static String WORKSPACE = "workspace.properties";

	private OBEDesigner parent;
	private File workspaceFile;

	public WorkSpaceManager(OBEDesigner parent)
	{
		this.parent = parent;
		this.workspaceFile =
			new File(System.getProperty("ide.home") + "/conf/" + WORKSPACE);
		
		if( !workspaceFile.exists())
		{
			saveProjects();
		}
	}
	
	/**
	 * 保存工程信息
	 * @param p
	 */
	public void saveProjects()
	{
		try
		{
			Iterator f =OBEModuleManager.getInstance().getProjectManager().
				getAllProjects().iterator();

			Document doc =new Document();
			Element root =new Element( WS_PROJECT);
			
			ProjectData pd =null;
			while( f.hasNext())
			{
				pd =( ProjectData)f.next();
				Element e =new Element( WS_PROJECTE);
				e =e.setAttribute( WS_PROJECTNAME, pd.getProjectName());
				e =e.setAttribute( WS_PROJECTPATH, pd.getPath());
				root =root.addContent( e);
			}

			doc =doc.setRootElement( root);
			XMLOutputter out =new XMLOutputter();
			FileOutputStream o =new FileOutputStream( workspaceFile);
			
			out.setEncoding( "UTF-8");
			out.setNewlines( true);
			out.setIndent( "\t");
			out.output( doc, o);
			
			o.close();
		}
		catch( Exception e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * 加载项目信息
	 */
	public void loadWorkProjects()
	{
		try
		{
			SAXBuilder builder =new SAXBuilder();
			Document doc =builder.build( workspaceFile);
			Element root =doc.getRootElement();
			Iterator f =root.getChildren( WS_PROJECTE).iterator();
			ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
			while( f.hasNext())
			{
				Element e =( Element)f.next();
				String n =e.getAttributeValue( WS_PROJECTNAME);
				String p =e.getAttributeValue( WS_PROJECTPATH);
				File ff =new File( p, n);
				if( !ff.isDirectory())
					continue;
				ProjectData pd =ProjectData.loadProject( n, p);
				if( pd == null)
					continue;
				//保存项目信息
				pm.newProject( pd);
				//加载项目信息
				pd.loadProject();
			}
		}
		catch( Exception e1){
			e1.printStackTrace();
		}
	}
	
	/**
	 * 更新信息树
	 */
	public void refrushWorkspaceTree()
	{
		//获取项目信息管理器
		ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
		Iterator f =pm.getAllProjects().iterator();
		ProjectData data =null;
		
		OBETree tree =parent.getTree();
		tree.removeAllFromRoot();
		OBETreeNode node =null;	
		while( f.hasNext())
		{
			data =( ProjectData)f.next();
			
			String mkey =data.getDefaultModuleKey();
			String uuid =data.getModuleUUID( mkey);
			//增加节点
			ElementKey ek =new ElementKey( data.getProjectID(), OBETreeNode.PROJECT, mkey, uuid);
			OBETreeData td =new OBETreeData( data.getProjectName(), OBETreeNode.PROJECT,
				"project.gif", ek.createKey());
			node =tree.addProjectNode( td);
			
			//递归加载模块数据
			if( node != null)
				data.refrushWorkspaceTree( tree, node);
			node =null;
		}
		
		//更新界面
		tree.updateUI();
	}

	public OBEDesigner getParent()
	{
		return parent;
	}

}
