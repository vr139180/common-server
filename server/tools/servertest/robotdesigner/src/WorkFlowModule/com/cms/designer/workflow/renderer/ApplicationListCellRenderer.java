package com.cms.designer.workflow.renderer;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JList;

import com.cms.core.workflow.application.Application;

public class ApplicationListCellRenderer extends DefaultListCellRenderer
{

	public Component getListCellRendererComponent( JList list, Object value, int index,
			boolean isSelected, boolean cellHasFocus)
	{
		super.getListCellRendererComponent( list, value, index, isSelected, cellHasFocus);

		Application app = (Application) value;
		setText( app.getName());

		return this;
	}

}
