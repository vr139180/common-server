package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * $Id: WorkflowFileNew.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class WorkflowFileNew extends OBEStandardAction
{
	private String key =null;
	
	public WorkflowFileNew(OBEDesigner parent)
	{
		super(parent);
	}

	/**
	 * @return
	 */
	public String getKey()
	{
		return key;
	}

	/**
	 * @param key
	 */
	public void setKey(String key)
	{
		this.key = key;
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		/*
		ElementKey ek =new ElementKey();
		ek.analyze( key);
		
		ProjectManager manager =OBEModuleManager.getInstance().getProjectManager();
		ProjectData pd =manager.getOneProject( ek.getProjectID());
		if( pd == null)	return;
		String uid =pd.getModuleUUID( OBEWorkflowModule.MODULE_ID);
		
		OBEWorkflowModule module =( OBEWorkflowModule)OBEModuleManager.getInstance().
			getModuleManager().getModule( OBEWorkflowModule.MODULE_ID);
		if( module == null)	return;
		WorkflowModuleData data =( WorkflowModuleData)module.getModuleData( uid);
		if( data == null)	return;
		
		TreeNodeKey nk =new TreeNodeKey();
		nk.analyee( ek.getModuleExtAtt());
		WorkflowPackage pk =data.getOnePackage( nk.getPackageID());
		if( pk == null && pk.getFile() != null) return;

		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		//保存各个元素的位置
		((OBEGraphWorkflow)frame.getGraph()).save( null);
		pk.savePackageFile();
		parent.setStatus("Saving workflow to " + pk.getFile().getAbsolutePath());*/
	}

}
