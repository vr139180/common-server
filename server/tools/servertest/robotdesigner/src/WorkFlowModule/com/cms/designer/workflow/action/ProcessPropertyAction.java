package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;
import com.cms.designer.workflow.TreeNodeKey;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.dialog.EditWorkflowProcessDialog;

/**
 * Process�༭����
 * @author today
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class ProcessPropertyAction extends OBEStandardAction
{
	/**
	 * @param parent
	 */
	public ProcessPropertyAction(OBEDesigner parent)
	{
		super(parent);
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		OBETreeData data =( OBETreeData)parent.getTree().getCurSelectedNode().getUserObject();
		if( data == null) return;
		ElementKey ek =new ElementKey();
		ek.analyze( data.getKey());
				
		//��ȡ������
		//��ȡ����Ϣ
		ModuleManager manager =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =manager.getModule( ek.getModuleID());
		if( ms == null)	return;
		//���������Ϣ
		WorkflowModuleData wmd =( WorkflowModuleData)ms.getModuleData( ek.getModuleExt());
		if( wmd == null) return;
		TreeNodeKey tk =new TreeNodeKey();
		tk.analyee( ek.getModuleExtAtt());
		
		WorkflowPackage wpackage =wmd.getOnePackage( tk.getProcessID());
		WorkflowProcess wp =wpackage.getWorkflowProcess( tk.getProcessID());
		if( wp == null) return;

		EditWorkflowProcessDialog d = new EditWorkflowProcessDialog( parent, wp, data.getKey(), "�༭���̶�������");
		if( d.showDialog() == EditWorkflowProcessDialog.APPROVE_OPTION)
		{
			//����tree��node��ʾ
			OBETreeNode node =parent.getTree().getCurSelectedNode();
			data.setName( wp.getName());
			parent.getTree().updateUI();
		}
	}

}
