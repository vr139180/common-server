package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class SystemQuit extends OBEStandardAction
{
	private OBEDesigner parent =null;
	
	public SystemQuit( OBEDesigner parent)
	{
		super(parent);
		
		this.parent =parent;
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		System.exit( 1);
	}

}
