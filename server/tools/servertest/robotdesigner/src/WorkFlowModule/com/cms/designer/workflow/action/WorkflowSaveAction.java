package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.validate.Validate;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.OBEWorkFlowFrame;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.TreeNodeKey;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.validate.Validator;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class WorkflowSaveAction extends OBEStandardAction
{
	private String key =null;
	
	public WorkflowSaveAction(OBEDesigner parent)
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

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);

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
		WorkflowPackage pk =data.getOnePackage( nk.getProcessID());
		if( pk == null && pk.getFile() != null) return;

		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		//保存各个元素的位置
		((OBEGraphWorkflow)frame.getGraph()).save(null);
		
		//增加监测
		pk.savePackageFile();
		
		parent.setStatus("Saving workflow to " + pk.getFile().getAbsolutePath());

		if( !checkWorkflowValidate(pd, data, pk, ((OBEGraphWorkflow)frame.getGraph()).getWorkflowProcess()))
		{
			JOptionPane.showMessageDialog(parent, "监测到设计的流程有问题！\r\n\t请参照提示信息进行修改", 
					"警告", JOptionPane.WARNING_MESSAGE);
		}
		
		frame.setFrameModified( false);
	}
	
	/**
	 * 20040811 york 修改 增加了 WorkflowModuleData data 和 ProjectData pd 参数 将在WorkflowProcessValidate中使用
	 * @param wp
	 * @return
	 */
	private boolean checkWorkflowValidate(ProjectData pd , WorkflowModuleData data, WorkflowPackage wp, WorkflowProcess process)
	{
		if( process == null)
			return true;
		
		boolean ret =false;

		Validate validate =new Validator(pd, data, wp, process);
		ret =validate.validate();
		if( !ret)
			this.getParent().fireCheckToolTipInfo( true, validate);
		
		return ret;
	}
}
