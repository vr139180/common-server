package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;

import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEWorkFlowFrame;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: WorkflowZoomoutAction.java,v 1.1 2004/06/30 06:36:47 jeffery Exp $
 */
public class WorkflowZoomoutAction extends OBEStandardAction
{
	private String key =null;
	
	public WorkflowZoomoutAction(OBEDesigner parent)
	{
		super(parent);
		// XXX Auto-generated constructor stub
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
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		
		frame.getGraph().zoomOut();
	}

}
