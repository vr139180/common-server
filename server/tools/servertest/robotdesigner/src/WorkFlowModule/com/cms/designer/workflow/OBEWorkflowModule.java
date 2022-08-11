package com.cms.designer.workflow;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.util.HashMap;
import java.util.Iterator;

import javax.swing.JMenu;
import javax.swing.JPopupMenu;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.parser.dom4j.Dom4JXPDLParser;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.OBEModule;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.OBEModuleData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.util.ConstValue;
import com.cms.designer.workflow.action.WorkflowEditCopy;
import com.cms.designer.workflow.action.WorkflowEditCut;
import com.cms.designer.workflow.action.WorkflowEditPaste;
import com.cms.designer.workflow.action.WorkflowEditSelectAll;
import com.cms.designer.workflow.action.WorkflowEditSelectNone;
import com.cms.designer.workflow.action.WorkflowSaveAction;
import com.cms.designer.workflow.action.WorkflowSaveasAction;
import com.cms.designer.workflow.action.WorkflowZoominAction;
import com.cms.designer.workflow.action.WorkflowZoomoutAction;
import com.cms.designer.workflow.graph.OBEGraphModel;

/**
 * @author Administrator
 * 工作流模块的入口
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEWorkflowModule.java,v 1.6 2004/09/02 09:06:18 jefferyd Exp $
 */
public class OBEWorkflowModule extends OBEModuleStandard
{
	//模块标示
	public final static String MODULE_ID ="designer.workflow";
	//工具条常量定义
	public final static String EDIT = "edit";

	public final static String LINE = "line";
	public final static String TOOLIMP ="toolimp";
	
	//保存注册的事件
	private HashMap actions =new HashMap();

	//菜单管理
	private WorkFlowMenuManager menuManager =null;
	
	public OBEWorkflowModule( OBEModule m, OBEDesigner p)
	{
		super( m, p);
		init();
	}
	
	/**
	 * 完成各种促使化任务
	 */
	protected void init()
	{
		//初始化菜单管理
		menuManager =new WorkFlowMenuManager( this);
		
		//注册工具条按钮
		registerWorkflowActions();
		registerToolbar();
		registerIDEActions();
	}

	/*
	1.init the special action HashMap:OBEModuleStandard.ideActions
	2.only actions put inside ideActions could be OBEModuleStandard.getIDEAction(String name)
	  and setKey().
	3.CoreFileOpen,CoreFileNew need not setKey, so never registerIDEActions()
	4.reference:
	  OBEFormFrame.changeView()
	  OBEWorkflowFrame.changeView()
	5.the means of OBEModuleStandard.ideActions:
	  1.faciliate for the button[menubar,toolbar] actions being changed into special action
	  2.make the action of one item from toolbar or menubar could be setKey()
	  */
	private void registerIDEActions()
	{
		OBEStandardAction action =null;
		/*
		act =new WorkflowFileNew( getParent());
		registerIDEAction( ConstValue.CoreFileNew, act);
		act =new WorkflowFileOpen( getParent());
		registerIDEAction( ConstValue.CoreFileOpen, act);*/
		action =new WorkflowSaveAction( getParent());
		registerIDEAction( ConstValue.CoreFileSave, action);
		action =new WorkflowSaveasAction( getParent());
		registerIDEAction( ConstValue.CoreFileSaveas, action);

		action=new WorkflowZoominAction(getParent());
		registerIDEAction(ConstValue.CoreViewZoomin, action);
		action=new WorkflowZoomoutAction(getParent());
		registerIDEAction(ConstValue.CoreViewZoomout, action);

		action=new WorkflowEditSelectAll(getParent());
		registerIDEAction(ConstValue.CoreEditSelectAll, action);
		action=new WorkflowEditSelectNone(getParent());
		registerIDEAction(ConstValue.CoreEditSelectNone, action);

		action=new WorkflowEditCut(getParent());
		registerIDEAction(ConstValue.CoreEditCut, action);
		action=new WorkflowEditCopy(getParent());
		registerIDEAction(ConstValue.CoreEditCopy, action);
		action=new WorkflowEditPaste(getParent());
		registerIDEAction(ConstValue.CoreEditPaste, action);
	}
	
	private void registerWorkflowActions()
	{
	}

	private void registerToolbar()
	{
		registerToolbarButton(
			EDIT,
			"edit.gif",
			"edit2.gif",
			ResourceUtil.getRS("window.edit"));
		
		registerToolbarButton(
			TOOLIMP,
			"tooldealup_16.gif",
			"tooldealup_16.gif",
			"工具自动处理");

		registerToolbarButton(
			LINE,
			"drawlineup_16.gif",
			"drawlineup_16.gif",
			"条件转移");
		
	}
	
	public OBEStandardAction getWorkflowAction( String n)
	{
		return ( OBEStandardAction)actions.get( n);
	}
	
	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#getModuleMenu()
	 */
	public JMenu getModuleMenu()
	{
		return menuManager.getSystemMenu();
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#getPopupMenu(com.excenon.designer.util.OBETreeData)
	 */
	public boolean getPopupMenu( JPopupMenu root, String moduleExtAtt)
	{
		return menuManager.getPopupMenu( root, moduleExtAtt);
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#newModuleData(java.lang.String, java.lang.String)
	 */
	public OBEModuleData newModuleData(String projectID, String moduleID)
	{
		WorkflowModuleData wmd =new WorkflowModuleData( moduleID, projectID);
		addModuleData( wmd);
		return wmd;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#newModuleFrame(java.lang.String)
	 */
	public OBEInternalFrame newModuleFrame(String key)
	{
		ElementKey ek =new ElementKey();
		ek.analyze( key);
		TreeNodeKey tk =new TreeNodeKey();
		tk.analyee( ek.getModuleExtAtt());
		WorkflowModuleData wmd =( WorkflowModuleData)getModuleData( ek.getModuleExt());
		if( wmd == null)
			return null;
		
		WorkflowPackage wp =wmd.getOnePackage( tk.getProcessID());
		if( wp == null)
			return null;
		WorkflowProcess p =wmd.getOneProcess( tk.getProcessID());
		
		OBEGraphWorkflow graph =new OBEGraphWorkflow( getParent(), new OBEGraphModel(),key);
		graph.setWorkflowPackage( wp);
		graph.setWorkflowProcess( p);
		OBEWorkFlowFrame frame =new OBEWorkFlowFrame( getParent(), this, graph, key, tk.getProcessID());
		
		return frame;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#canOpenView(java.lang.String)
	 */
	public boolean canOpenView(String moduleExtAtt)
	{
		boolean ret =false;
		if( moduleExtAtt == null || moduleExtAtt.equals( ""))
			return ret;
		TreeNodeKey tk =new TreeNodeKey();
		tk.analyee( moduleExtAtt);
		if( tk.getProcessID() != null && !tk.getProcessID().equals( ""))
			ret =true;

		return ret;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.OBEModuleStandard#loadModuleData(java.lang.String, java.lang.String)
	 */
	public OBEModuleData loadModuleData(String path, String projectID)
	{
		File f =new File( path, getOBEModule().getWorkDir());
		WorkflowModuleData data =new WorkflowModuleData( OBEWorkflowModule.MODULE_ID, projectID);
		data.setWorkDir( f);
		
		//保存模块数据信息
		addModuleData( data);
		
		//分析目录加载包信息
		File[] dd =f.listFiles( new FileFilter(){
			public boolean accept(File pathname)
			{
				// XXX Auto-generated method stub
				String n =pathname.getName().toLowerCase();
				return n.endsWith( ".xpdl");
			}});
		
		for( int i =0; i<dd.length; ++i)
		{
			if( !dd[i].isFile())
				continue;
			//根据目录名称分析包信息
			WorkflowPackage pp =loadPackageFromFile( dd[i]);
			if( pp == null)
				continue;
			//增加包信息
			data.addPackage( pp);
		}
		
		return data;
	}
	
	/**
	 * 加载包信息
	 * @param f
	 * @return
	 */
	private WorkflowPackage loadPackageFromFile( File f)
	{
		WorkflowPackage ret =null;
		FileInputStream fin =null;
		try
		{
			Dom4JXPDLParser parser =new Dom4JXPDLParser();
			fin =new FileInputStream( f);
			ret =parser.parse( fin);
			ret.setFile( f);
			fin.close();
			fin =null;
		}
		catch( Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			if( fin != null)
			{
				try
				{
					fin.close();
				}
				catch( Exception ee){
				}
			}
		}

		return ret;
	}
	public WorkflowPackage findWP(WorkflowProcess process){
	


		WorkflowPackage wp = null;
		Iterator iterator = super.getModuleUuid();

		while (iterator.hasNext()){
			String key = (String) iterator.next();
			Object data =(Object) super.getModuleData(key);
			wp =  ((WorkflowModuleData)data).findpackage(process);
			if (wp!=null) break;
		}		
		return wp;
	}
}
