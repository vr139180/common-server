package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author excenon
 * $Id: HelpAbout.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class HelpAbout extends OBEStandardAction
{

	/**
	 * @param parent
	 */
	public HelpAbout( OBEDesigner parent)
	{
		super( parent);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		JOptionPane.showConfirmDialog( null,"version:1.0\nCopyright:华磊科技","关于对话框",JOptionPane.DEFAULT_OPTION);
		return;
	}
}
