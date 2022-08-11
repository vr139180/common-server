package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETree;
import com.cms.designer.swingui.OBETreeNode;

/**
 * @author Administrator
 * $Id: WorkflowFileOpen.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class WorkflowFileOpen extends OBEStandardAction
{
	private String key =null;
	
	public WorkflowFileOpen(OBEDesigner parent)
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

		OBETree tree=parent.getTree();
		OBETreeNode node=(OBETreeNode)tree.getLastSelectedPathComponent();
		if(node==null) return;
		
		tree.openModuleWindow(node);
		return;
	}

}
