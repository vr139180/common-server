package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * $Id: FileOpen.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class FileOpen extends OBEStandardAction
{
	public FileOpen(OBEDesigner parent)
	{
		super(parent);
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);

		//import one project
		(new ProjectImport(parent)).actionPerformed(e);
		return;
	}

}
