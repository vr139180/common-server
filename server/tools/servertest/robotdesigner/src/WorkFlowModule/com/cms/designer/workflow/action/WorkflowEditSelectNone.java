package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEWorkFlowFrame;

/**
 * @author Administrator
 * $Id: WorkflowEditSelectNone.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class WorkflowEditSelectNone extends OBEStandardAction
{
	private String key =null;
	public WorkflowEditSelectNone( OBEDesigner parent)
	{
		super(parent);
	}
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		System.out.print("WorkflowEditSelectNone.java-->");
		
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		
		frame.getGraph().selectNone();
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
