package com.cms.designer.workflow;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.OBEModuleData;
import com.cms.designer.swingui.OBETree;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: WorkflowModuleData.java,v 1.8 2004/08/19 08:49:08 york Exp $
 */
public class WorkflowModuleData extends OBEModuleData
{
	//保存流程的信息
	private HashMap packages =new HashMap();
	//工作目录
	private File workDir =null;

	public WorkflowModuleData( String moduleID, String projectID)
	{
		super( moduleID, projectID);
	}

	/**
	 * @param n
	 * @return
	 */
	public boolean containProcess( String n)
	{
		return packages.containsKey( n.toLowerCase());
	}
	
	/**
	 * 删除一个包
	 * @param id
	 * @return
	 */
	public boolean removeOneProcess( String id)
	{
		boolean ret =true;
		id =id.toLowerCase();
		WorkflowPackage wp =( WorkflowPackage)packages.get( id);
		if( wp == null)
			return ret;
		if( wp.getFile() != null)
			ret =wp.getFile().delete();

		if( ret)
			packages.remove( id);
		
		return ret;
	}
	
	public void addPackage( WorkflowPackage wp)
	{
		String n =wp.getName().toLowerCase();
		if( packages.containsKey( n))
			return;
		packages.put( n, wp);
	}
	/**
	 * @param wp
	 * @return
	 */
	public boolean addOneProcess( WorkflowProcess wp)
	{
		String n =wp.getName();
		n =n.toLowerCase();
		if( packages.containsKey( n))
			return false;

		WorkflowPackage wpackage =  new WorkflowPackage( "this.id.will.be.repalce", wp.getName());
		wpackage.initPackage();
		wpackage.getWorkflowProcesses().add(wp);

		packages.put( n, wpackage);
		
		//创建一个xpdl文件
		try
		{
			File f =new File( workDir, n+".xpdl");
			if( !f.exists())
			{
				f.createNewFile();
				wpackage.setFile( f);
				wpackage.savePackageFile();
			}
			wpackage.setFile( f);
		}
		catch( IOException io)
		{
			io.printStackTrace();
			packages.remove( n);
			return false;
		}
		
		return true;
	}
	
	/**
	 * 从一个包里面取出一个过程
	 * @param pid
	 * @param process
	 * @return
	 */
	public WorkflowProcess getOneProcess( String pid)
	{
		WorkflowProcess ret =null;

		pid =pid.toLowerCase();
		WorkflowPackage pk =( WorkflowPackage)packages.get( pid);
		if( pk == null)	
			return ret;
		
		List ll =pk.getWorkflowProcesses();
		if( ll.size() == 0)
			return ret;
		
		return ( WorkflowProcess)ll.get( 0);
	}
	
	public WorkflowPackage getOnePackage( String pid)
	{
		WorkflowPackage pk =( WorkflowPackage)packages.get( pid);
		return pk;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.workspace.OBEModuleData#mkModuleDir(java.lang.String)
	 */
	public void mkModuleDir(String parent)
	{
		//创建模块数据保存大额目录
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =mm.getModule( getModuleID());
		String wp =ms.getOBEModule().getWorkDir();
		workDir =new File( parent, wp);
		workDir.mkdir();
	}
	
	/**
	 * 获取工作目录
	 * @return
	 */
	public File getWorkDir()
	{
		return workDir;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.workspace.OBEModuleData#refrushWorkspaceTree(com.excenon.designer.swingui.OBETree, com.excenon.designer.swingui.OBETreeNode)
	 */
	public void refrushWorkspaceTree(OBETree tree, OBETreeNode parent)
	{
		OBETreeData dd =( OBETreeData)parent.getUserObject();
		ElementKey ek =new ElementKey();
		ek.analyze( dd.getKey());
		
		Iterator ff =packages.values().iterator();
		WorkflowPackage wp =null;
		while( ff.hasNext())
		{
			wp =( WorkflowPackage)ff.next();

			//增加过程信息
			TreeNodeKey tk =new TreeNodeKey();
			tk.setProcessID( wp.getName());
			ek.setModuleExtAtt( tk.createKey());
			
			OBETreeData td =new OBETreeData( wp.getName(), OBETreeNode.MODULE,
				"process_tree1.gif", ek.createKey());
			tree.addProjectModuleNode( parent, td);
		}
	}

	/**
	 * @param workDir
	 */
	public void setWorkDir(File workDir)
	{
		this.workDir = workDir;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.workspace.OBEModuleData#defaultModuleInit(com.excenon.designer.swingui.OBETree, com.excenon.designer.swingui.OBETreeNode)
	 */
	public void defaultModuleInit( OBETree tree, OBETreeNode parent)
	{
	}
	
	public void distroy()
	{
		deleteFile(workDir);
	}
	
	/**
	 * 查找包含该工作流的包
	 * @param pross
	 * @return
	 */
	public WorkflowPackage findpackage(WorkflowProcess pross){
		
		Iterator iterator = packages.keySet().iterator();
		while (iterator.hasNext()){
			Object key = iterator.next();
			WorkflowPackage workpackage = (WorkflowPackage)packages.get(key);
			if (workpackage.getWorkflowProcess(pross.getName())!=null)
				return workpackage;
		}
		return null;
	} 
}
