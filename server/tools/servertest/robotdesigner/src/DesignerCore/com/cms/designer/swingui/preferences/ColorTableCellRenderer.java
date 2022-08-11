package com.cms.designer.swingui.preferences;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;

public class ColorTableCellRenderer extends DefaultTableCellRenderer
{

	public Component getTableCellRendererComponent( JTable table, Object value, boolean isSelected,
			boolean hasFocus, int row, int column)
	{
		super.getTableCellRendererComponent( table, value, isSelected, hasFocus, row, column);

		if( value instanceof Color)
		{
			setBackground( (Color) value);
		}

		setText( "");

		return this;
	}

}
