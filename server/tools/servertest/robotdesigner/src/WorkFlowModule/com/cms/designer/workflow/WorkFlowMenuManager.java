package com.cms.designer.workflow;

import java.util.HashMap;

import javax.swing.Action;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.OBEModule;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.workflow.action.DeleteWorkflowGroup;
import com.cms.designer.workflow.action.DeleteWorkflowProcess;
import com.cms.designer.workflow.action.DeployProcess;
import com.cms.designer.workflow.action.ProcessPropertyAction;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: WorkFlowMenuManager.java,v 1.4 2004/08/12 02:47:37 jefferyd Exp $
 */
public class WorkFlowMenuManager
{
	//用到的action常量定义
	public final static String ACT_NEWPROCESS ="wfact.newprocess";
	public final static String ACT_DEPLOY ="wfact.deploy";
	public final static String ACT_PROCESSPROPERTY ="wfact.property";
	public final static String ACT_DELETEGROUP ="wfact.deletepackage";
	public final static String ACT_DELETEPROCESS ="wfact.deleteprocess";
	
	public final static String MENU_NEWMENU ="wf.newmenu";

	private OBEWorkflowModule parent =null;
	
	//系统菜单
	private JMenu workflowMenu;
	//编辑
	private JMenuItem workflowEditItem;
	//标签
	private JMenuItem workflowTagItem;
	//直线
	private JMenuItem workflowLineItem;
	
	//弹出式菜单
	private JMenu newMenu;
	//发布
	private JMenuItem deployItem;
	//过程属性
	private JMenuItem processPropertyItem;
	//delete form workflow group
	private JMenuItem deleteGroupItem;
	//删除过程
	private JMenuItem deleteProcessItem;
	
	//存放响应的动作
	private HashMap actions =new HashMap();
	
	public WorkFlowMenuManager( OBEWorkflowModule p)
	{
		super();
		parent =p;
		
		//初始化菜单
		init();
	}
	
	/**
	 * 初始化菜单信息
	 */
	private void init()
	{
		OBEModule md =parent.getOBEModule();
		//初始化系统菜单项
		workflowMenu =new JMenu();
		workflowEditItem =new JMenuItem( );
		workflowMenu.add( workflowEditItem);
		workflowTagItem =new JMenuItem( );
		workflowMenu.add( workflowTagItem);
		workflowLineItem =new JMenuItem( );
		workflowMenu.add( workflowLineItem);
		
		//初始化存放响应的动作
		OBEStandardAction act =null;
		//过程属性
		act =new ProcessPropertyAction( parent.getParent());
		actions.put( ACT_PROCESSPROPERTY, act);
		//发布过程
		act =new DeployProcess( parent.getParent());
		actions.put( ACT_DEPLOY, act);
		//action to delete group
		act =new DeleteWorkflowGroup( parent.getParent());
		actions.put( ACT_DELETEGROUP, act);
		//删除过程
		act =new DeleteWorkflowProcess( parent.getParent());
		actions.put( ACT_DELETEPROCESS, act);
	}
	
	/**
	 * 加载菜单资源
	 */
	private void loadResource()
	{
		OBEModule md =parent.getOBEModule();
		if( newMenu != null)
			newMenu.setText( md.getResource( MENU_NEWMENU));
		
		if( deployItem != null)
			deployItem.setText( md.getResource( ACT_DEPLOY));

		if( deleteGroupItem != null)
			deleteGroupItem.setText( md.getResource( ACT_DELETEGROUP));

		if( processPropertyItem != null)
			processPropertyItem.setText( md.getResource( ACT_PROCESSPROPERTY));
			
		if( deleteProcessItem != null)
			deleteProcessItem.setText( md.getResource( ACT_DELETEPROCESS));
	}
	
	/**
	 * 重置状态
	 */
	private void resetMenuItem()
	{
		newMenu =null;
		deployItem =null;
		processPropertyItem =null;
		deleteProcessItem =null;
	}

	/**
	 * 获取系统菜单
	 * @return
	 */
	public JMenu getSystemMenu()
	{
		return workflowMenu;
	}
	
	/**
	 * 根据不同的情况获取不同的菜单
	 * @param d
	 * @return
	 */
	public boolean getPopupMenu( JPopupMenu root, String key)
	{
		boolean b =true;
		resetMenuItem();
		
		ElementKey ek =new ElementKey();
		ek.analyze( key);
		String moduleExtAtt =ek.getModuleExtAtt();
		
		if( moduleExtAtt == null || moduleExtAtt.equals( ""))
		{
			//根菜单
			newMenu =new JMenu();
			root.add( newMenu);
			
			deleteGroupItem =root.add( ( Action)actions.get( ACT_DELETEGROUP));
		}
		else
		{
			TreeNodeKey tk =new TreeNodeKey();
			tk.analyee( moduleExtAtt);
			if( tk.getProcessID() != null && !tk.getProcessID().equals( ""))
			{
				//处理流程的菜单
				deployItem =root.add( ( Action)actions.get( ACT_DEPLOY));
				root.addSeparator();
				deleteProcessItem =root.add( ( Action)actions.get( ACT_DELETEPROCESS));
				processPropertyItem =root.add( ( Action)actions.get( ACT_PROCESSPROPERTY));
			}
		}
		
		loadResource();
		
		return b;
	}
	
	/**
	 * 根据id获取响应的动作
	 * @param id
	 * @return
	 */
	public Action getAction( String id)
	{
		return ( Action)actions.get( id);
	}
}
