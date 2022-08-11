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
	//�õ���action��������
	public final static String ACT_NEWPROCESS ="wfact.newprocess";
	public final static String ACT_DEPLOY ="wfact.deploy";
	public final static String ACT_PROCESSPROPERTY ="wfact.property";
	public final static String ACT_DELETEGROUP ="wfact.deletepackage";
	public final static String ACT_DELETEPROCESS ="wfact.deleteprocess";
	
	public final static String MENU_NEWMENU ="wf.newmenu";

	private OBEWorkflowModule parent =null;
	
	//ϵͳ�˵�
	private JMenu workflowMenu;
	//�༭
	private JMenuItem workflowEditItem;
	//��ǩ
	private JMenuItem workflowTagItem;
	//ֱ��
	private JMenuItem workflowLineItem;
	
	//����ʽ�˵�
	private JMenu newMenu;
	//����
	private JMenuItem deployItem;
	//��������
	private JMenuItem processPropertyItem;
	//delete form workflow group
	private JMenuItem deleteGroupItem;
	//ɾ������
	private JMenuItem deleteProcessItem;
	
	//�����Ӧ�Ķ���
	private HashMap actions =new HashMap();
	
	public WorkFlowMenuManager( OBEWorkflowModule p)
	{
		super();
		parent =p;
		
		//��ʼ���˵�
		init();
	}
	
	/**
	 * ��ʼ���˵���Ϣ
	 */
	private void init()
	{
		OBEModule md =parent.getOBEModule();
		//��ʼ��ϵͳ�˵���
		workflowMenu =new JMenu();
		workflowEditItem =new JMenuItem( );
		workflowMenu.add( workflowEditItem);
		workflowTagItem =new JMenuItem( );
		workflowMenu.add( workflowTagItem);
		workflowLineItem =new JMenuItem( );
		workflowMenu.add( workflowLineItem);
		
		//��ʼ�������Ӧ�Ķ���
		OBEStandardAction act =null;
		//��������
		act =new ProcessPropertyAction( parent.getParent());
		actions.put( ACT_PROCESSPROPERTY, act);
		//��������
		act =new DeployProcess( parent.getParent());
		actions.put( ACT_DEPLOY, act);
		//action to delete group
		act =new DeleteWorkflowGroup( parent.getParent());
		actions.put( ACT_DELETEGROUP, act);
		//ɾ������
		act =new DeleteWorkflowProcess( parent.getParent());
		actions.put( ACT_DELETEPROCESS, act);
	}
	
	/**
	 * ���ز˵���Դ
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
	 * ����״̬
	 */
	private void resetMenuItem()
	{
		newMenu =null;
		deployItem =null;
		processPropertyItem =null;
		deleteProcessItem =null;
	}

	/**
	 * ��ȡϵͳ�˵�
	 * @return
	 */
	public JMenu getSystemMenu()
	{
		return workflowMenu;
	}
	
	/**
	 * ���ݲ�ͬ�������ȡ��ͬ�Ĳ˵�
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
			//���˵�
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
				//�������̵Ĳ˵�
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
	 * ����id��ȡ��Ӧ�Ķ���
	 * @param id
	 * @return
	 */
	public Action getAction( String id)
	{
		return ( Action)actions.get( id);
	}
}
