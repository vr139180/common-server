package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.dialog.ProjectExportDialog;
import com.cms.designer.swingui.dialog.ProjectImportDialog;

/**
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: ProjectImport.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class ProjectImport extends OBEStandardAction
{
	public ProjectImport( OBEDesigner parent)
	{
		super(parent);
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed( ActionEvent e)
	{
		super.actionPerformed( e);
		ProjectImportDialog dialog = new ProjectImportDialog(parent);
		if (dialog.showDialog() == ProjectExportDialog.APPROVE_OPTION)
		{
			if( !dialog.save())
			{
				JOptionPane.showMessageDialog(parent, "导入项目失败", "警告", JOptionPane.OK_OPTION);
			}
		}
	}
}
