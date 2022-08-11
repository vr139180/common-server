package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEWorkFlowFrame;

/**
 * @author Administrator
 * $Id: WorkflowEditCut.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class WorkflowEditCut extends OBEStandardAction
{
	private String key =null;
	public WorkflowEditCut( OBEDesigner parent)
	{
		super(parent);
	}
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		System.out.print("WorkflowEditCut.java-->");
		
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		
		frame.getGraph().cut();
		return;
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
}
