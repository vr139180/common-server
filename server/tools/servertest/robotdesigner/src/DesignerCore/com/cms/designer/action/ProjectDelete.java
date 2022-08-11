package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;
import javax.swing.tree.TreePath;

import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;

/**
 * @author Administrator
 * 
 * To change the template for this generated type comment go to Window -
 * Preferences - Java - Code Generation - Code and Comments $Id:
 * ProjectDelete.java,v 1.1.2.2 2004/06/08 02:57:25 gaj Exp $ $Id:
 * ProjectDelete.java,v 1.1.2.2 2004/06/08 02:57:25 gaj Exp $
 */
public class ProjectDelete extends OBEStandardAction
{
	public ProjectDelete( OBEDesigner parent)
	{
		super( parent);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed( ActionEvent e)
	{
		super.actionPerformed( e);

		OBETreeNode node = parent.getTree().getCurSelectedNode();
		if( node == null)
			return;

		int flag = -1;
		//return value:
		//0,delete operation confirmed
		//1,delete operation canceled
		flag = JOptionPane.showConfirmDialog( null, "确实要删除:"
				+ ((OBETreeData) node.getUserObject()).getName() + "吗?", "确认删除",
				JOptionPane.YES_OPTION);
		if( flag > 0)
			return;

		removeThisNode( node, parent);
		//refresh config file workspace.properties
		parent.getWorkSpaceManager().saveProjects();
		return;
	}

	public static void removeThisNode( OBETreeNode node, OBEDesigner parent)
	{
		OBETreeData data = (OBETreeData) node.getUserObject();
		ProjectManager pm = OBEModuleManager.getInstance().getProjectManager();
		ElementKey ek = new ElementKey();
		ek.analyze( data.getKey());
		pm.removeOneProject( ek.getProjectID());

		TreePath tp = parent.getTree().getSelectionPath();
		TreePath p = tp.getParentPath();
		node.removeFromParent();
		if( p != null)
		{
			OBETreeNode tn = (OBETreeNode) p.getLastPathComponent();
			if( tn.getChildCount() > 0)
			{
				OBETreeNode tt = (OBETreeNode) tn.getChildAt( tn.getChildCount() - 1);
				p = new TreePath( tt.getPath());
			}
			parent.getTree().setSelectionPath( p);
		}
		parent.getTree().updateUI();
		return;
	}
}
