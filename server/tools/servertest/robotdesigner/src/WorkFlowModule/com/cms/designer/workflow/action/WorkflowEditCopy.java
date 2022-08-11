package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEWorkFlowFrame;

/**
 * @author Administrator
 * $Id: WorkflowEditCopy.java,v 1.2 2004/09/02 09:06:18 jefferyd Exp $
 */
public class WorkflowEditCopy extends OBEStandardAction
{
	private String key =null;
	public WorkflowEditCopy( OBEDesigner parent)
	{
		super(parent);
	}
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		
		frame.getGraph().copy();
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
