package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.Icon;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author dolphin_zhou
 * $Id: EditCut.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class EditCut extends OBEStandardAction
{
	public EditCut(OBEDesigner parent)
	{
		super(parent);
	}

	public EditCut(OBEDesigner parent, String s)
	{
		super(parent, s);
	}

	public EditCut(OBEDesigner parent, String s, Icon icon)
	{
		super(parent, s, icon);
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		return;
	}
}
