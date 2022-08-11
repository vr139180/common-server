package com.cms.designer.swingui.preferences;

import java.awt.Color;

import javax.swing.table.AbstractTableModel;

import com.anthonyeden.lib.util.XArrayList;

public class ColorTableModel extends AbstractTableModel
{
	private static final String[] columnNames = {"Object", "Color"};

	private static final Class[] columnClasses = {String.class, Color.class};

	private XArrayList colorList;

	public ColorTableModel( XArrayList colorList)
	{
		this.colorList = colorList;
	}

	public int getColumnCount()
	{
		return columnNames.length;
	}

	public int getRowCount()
	{
		return colorList.size();
	}

	public String getColumnName( int index)
	{
		return columnNames[index];
	}

	public Class getColumnClass( int index)
	{
		return columnClasses[index];
	}

	public Object getValueAt( int row, int column)
	{
		ColorMap colorMap = (ColorMap) colorList.get( row);
		switch( column)
		{
		case 0:
			return colorMap.getName();
		case 1:
			return colorMap.getColor();
		default:
			throw new IllegalArgumentException( "Invalid column index");
		}
	}

}
