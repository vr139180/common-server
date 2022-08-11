package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.ConstValue;

/**
 * @author zagio
 * $Id: FileNew.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class FileNew extends OBEStandardAction
{
	public FileNew(OBEDesigner parent)
	{
		super(parent);
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		//create one new project
		((OBEStandardAction)parent.getActions().get( ConstValue.ProjectNew)).actionPerformed(e);

		return;
	}

}
