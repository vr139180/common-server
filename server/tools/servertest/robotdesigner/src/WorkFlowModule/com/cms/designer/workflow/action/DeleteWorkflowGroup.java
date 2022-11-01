package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;
import java.io.File;

import javax.swing.JOptionPane;
import javax.swing.tree.TreePath;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;
import com.cms.designer.workflow.WorkflowModuleData;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: DeleteWorkflowGroup.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class DeleteWorkflowGroup extends OBEStandardAction
{
	public DeleteWorkflowGroup(OBEDesigner parent)
	{
		super(parent);
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);

		OBETreeNode node =parent.getTree().getCurSelectedNode();
		if( node == null)
			return;

		int flag=-1;
		//return value:
		//0,delete operation confirmed
		//1,delete operation canceled
	  	flag=JOptionPane.showConfirmDialog(null,"确实要删除:"+((OBETreeData)node.getUserObject()).getName()+"吗?","确认删除",JOptionPane.YES_OPTION);
		if(flag>0)	return;

		removeThisNode(node,parent);
		return;
	}
	
	public static void removeThisNode(OBETreeNode node,OBEDesigner parent){
		OBETreeData data =( OBETreeData)node.getUserObject();
		File[] groupFiles =null;
		if( data == null)	return;
		ElementKey ek =new ElementKey();
		ek.analyze( data.getKey());
		
		//操作对应的包
		ModuleManager manager =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =manager.getModule( ek.getModuleID());
		if( ms == null)	return;

		WorkflowModuleData wmd =( WorkflowModuleData)ms.getModuleData( ek.getModuleExt());
		if( wmd == null) return;
		
		ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
		ProjectData pd =pm.getOneProject( ek.getProjectID());
		pd.removeOneProject(ek.getModuleID());

		wmd.distroy();
		
		TreePath tp =parent.getTree().getSelectionPath();
		TreePath p =tp.getParentPath();
		node.removeFromParent();
		if( p != null)
		{
			OBETreeNode tn =( OBETreeNode)p.getLastPathComponent();
			if( tn.getChildCount() >0)
			{
				OBETreeNode tt =( OBETreeNode)tn.getChildAt( tn.getChildCount()-1);
				p =new TreePath( tt.getPath());
			}
			parent.getTree().setSelectionPath( p);
		}
		parent.getTree().updateUI();
	}
}
