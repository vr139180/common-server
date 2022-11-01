package com.cms.designer.workflow;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JInternalFrame;
import javax.swing.JScrollPane;

import com.cms.designer.action.FileClose;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.swingui.OBEViewPanel;
import com.cms.designer.swingui.menu.OBEViewPopup;
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
import com.cms.designer.workflow.beacon.OBEWorkFlowFrameBeacon;
import com.cms.designer.workflow.event.CreateLineTransitionListener;
import com.cms.designer.workflow.event.CreateRoundTransitionListener;
import com.cms.designer.workflow.event.CreateTargetListener;
import com.cms.designer.workflow.event.GraphEditListener;

/**
 * @author Administrator
 * 
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEWorkFlowFrame.java,v 1.2 2004/06/30 09:19:07 jefferyd Exp $
 * $Id: OBEWorkFlowFrame.java,v 1.2 2004/06/30 09:19:07 jefferyd Exp $
 */
public class OBEWorkFlowFrame extends OBEInternalFrame
{
	private OBEGraphWorkflow graph = null;
	
	private JScrollPane scrollPane;
	private OBEViewPanel viewPanel;
	private JButton handButton;

	private OBEViewPopup viewPopup;
	
	//监听器
	private CreateTargetListener targetListener =null;
	private CreateLineTransitionListener lineListener =null;
	private GraphEditListener editListener =null;
	private CreateRoundTransitionListener roundListener =null;

	public OBEWorkFlowFrame( OBEDesigner parent, OBEModuleStandard module, 
		OBEGraphWorkflow graph, String key, String title)
	{
		super( parent, module, title);
		this.graph = graph;
		this.graph.setFrame( this);
		
		this.setName(key);
		graph.setName(key);
		module.setBeacon(new OBEWorkFlowFrameBeacon(parent));
		
		init1();
	}

	/**
	 * 初始化界面
	 */
	private void init1()
	{
		this.scrollPane = new JScrollPane(graph);
		
		handButton = new JButton();
		handButton.setFocusPainted(false);
		handButton.setIcon( iconManager.getIcon("hand.gif"));
		//缩略图
		handButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				viewPopup.show( handButton,	-OBEViewPopup.dim.width, -OBEViewPopup.dim.height);
			}
		});

		scrollPane.setCorner(JScrollPane.LOWER_RIGHT_CORNER, handButton);
		scrollPane.setHorizontalScrollBarPolicy( JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		scrollPane.setVerticalScrollBarPolicy( JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		this.getContentPane().add(scrollPane);
		
		//缩放菜单初始化
		viewPopup =
			new OBEViewPopup( getDesignerParent(), this.getScrollPane().getViewport(), graph);
		
		//设置监听器
		targetListener =new CreateTargetListener( graph, getModule());
		graph.addMouseListener( targetListener);
		graph.addMouseMotionListener( targetListener);
		
		lineListener =new CreateLineTransitionListener( graph, getModule());
		graph.addMouseListener( lineListener);
		graph.addMouseMotionListener( lineListener);
		
		editListener =new GraphEditListener( graph, getModule());
		graph.addMouseListener( editListener);
		graph.addMouseMotionListener( editListener);

		roundListener =new CreateRoundTransitionListener( graph, getModule());
		graph.addMouseListener( roundListener);
		graph.addMouseMotionListener( roundListener);
	}

	public OBEGraphWorkflow getGraph()
	{
		return graph;
	}
	
	public JScrollPane getScrollPane()
	{
		return scrollPane;
	}
	
	public void setGraph( OBEGraphWorkflow graph)
	{
		this.graph = graph;
	}
	
	public void setSelected()
	{
		try
		{
			this.setSelected(true);
//			getDesignerParent().getDesktopPanel().setAgentGraph(graph);
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}

	public void revert()
	{
	}

	public String toString()
	{
		return "OBEInternalFrame[ " + this.getName() + " ]";
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.swingui.OBEInternalFrame#changeView(javax.swing.JInternalFrame)
	 */
	public void changeView(JInternalFrame frame)
	{
		super.changeView(frame);
		
		OBEStandardAction action=null;
		OBEWorkflowModule module =( OBEWorkflowModule)OBEModuleManager.getInstance().getModuleManager().getModule( OBEWorkflowModule.MODULE_ID);
		if( module == null)	return;

		//set the module[frame] key properties for the special action
		action =(WorkflowEditCut)module.getIDEAction( ConstValue.CoreEditCut);
		if( action != null)
			((WorkflowEditCut)action).setKey( getName());
		action =(WorkflowEditCopy)module.getIDEAction( ConstValue.CoreEditCopy);
		if( action != null)
			((WorkflowEditCopy)action).setKey( getName());
		action =(WorkflowEditPaste)module.getIDEAction( ConstValue.CoreEditPaste);
		if( action != null)
			((WorkflowEditPaste)action).setKey( getName());

		action =(WorkflowEditSelectAll)module.getIDEAction( ConstValue.CoreEditSelectAll);
		if( action != null)
			((WorkflowEditSelectAll)action).setKey( getName());
		action =(WorkflowEditSelectNone)module.getIDEAction( ConstValue.CoreEditSelectNone);
		if( action != null)
			((WorkflowEditSelectNone)action).setKey( getName());

		action =(WorkflowSaveAction)module.getIDEAction( ConstValue.CoreFileSave);
		if( action!= null)
			((WorkflowSaveAction)action).setKey( getName());
		action =(WorkflowSaveasAction)module.getIDEAction( ConstValue.CoreFileSaveas);
		if( action != null)
			((WorkflowSaveasAction)action).setKey( getName());
		action =(FileClose)module.getParent().getOBEMenuBar().getByName(ConstValue.CoreFileClose).getAction();
		if( action != null)
			((FileClose)action).setKey( getName());

		action =(WorkflowZoominAction)module.getIDEAction( ConstValue.CoreViewZoomin);
		if( action != null)
			((WorkflowZoominAction)action).setKey( getName());
		action =(WorkflowZoomoutAction)module.getIDEAction( ConstValue.CoreViewZoomout);
		if( action != null)
			((WorkflowZoomoutAction)action).setKey( getName());
	}

}