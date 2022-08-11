package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.dialog.NewProjectDialog;
import com.cms.designer.swingui.dialog.ProjectExportDialog;

/**
 * @author zagio $Id: ProjectExport.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class ProjectExport extends OBEStandardAction
{

	public ProjectExport( OBEDesigner parent)
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
		ProjectExportDialog dialog = new ProjectExportDialog( parent);
		if( dialog.showDialog() == NewProjectDialog.APPROVE_OPTION)
		{
		}
	}
}
