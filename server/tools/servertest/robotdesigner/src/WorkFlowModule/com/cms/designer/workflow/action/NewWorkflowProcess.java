package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;
import com.cms.designer.workflow.TreeNodeKey;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.dialog.NewWorkflowProcessDialog;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: NewWorkflowProcess.java,v 1.4 2004/08/16 06:24:22 jefferyd Exp $
 */
public class NewWorkflowProcess extends OBEStandardAction
{
	public NewWorkflowProcess(OBEDesigner parent)
	{
		super(parent);
	}

	public void actionPerformed(ActionEvent e)
	{
		try
		{
			NewWorkflowProcessDialog dialog = new NewWorkflowProcessDialog(parent);
			if (dialog.showDialog() == NewWorkflowProcessDialog.APPROVE_OPTION)
			{
				OBETreeData data = (OBETreeData)parent.getTree().getCurSelectedNode().getUserObject();
				if (data == null)
					return;
				ElementKey ek = new ElementKey();
				ek.analyze(data.getKey());

				WorkflowProcess wp = dialog.getWorkflowProcess();
				//��ȡģ����Ϣ
				ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
				ProjectData pd =pm.getOneProject( ek.getProjectID());
				if( pd == null)
					return ;

				ModuleManager manager = OBEModuleManager.getInstance().getModuleManager();
				OBEModuleStandard ms = manager.getFirstModule();
				
				if (ms == null)	return;
				
				WorkflowModuleData wmd = (WorkflowModuleData)ms.getModuleData(ek.getModuleExt());
				if (wmd == null)
					return;

				//���������Ϣ
				if (!wmd.addOneProcess( wp)) return;
				
				//���ӹ�����Ϣ��tree��
				TreeNodeKey tk = new TreeNodeKey();
				tk.setProcessID( wp.getName());
				
				ek.setModuleExtAtt( tk.createKey());
				
				OBETreeData td = new OBETreeData(wp.getName(), OBETreeNode.MODULE, "process_tree1.gif", ek.createKey());
				parent.getTree().addProjectModuleNode(parent.getTree().getCurSelectedNode(), td);

				OBEInternalFrame frame = ms.newModuleFrame(ek.createKey());
				if (frame != null)
					parent.getDesktopPanel().addInternalFrame( frame, true);
			}
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}
}
