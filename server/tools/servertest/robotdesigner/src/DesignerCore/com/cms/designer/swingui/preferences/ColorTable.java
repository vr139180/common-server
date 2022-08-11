package com.cms.designer.swingui.preferences;

import java.awt.Color;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JTable;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class ColorTable extends JTable
{
	private static final Log log = LogFactory.getLog( ColorTable.class);

	public ColorTable( ColorTableModel model)
	{
		super( model);

		setDefaultRenderer( Color.class, new ColorTableCellRenderer());

		addMouseListener( new MouseAdapter()
		{
			public void mouseClicked( MouseEvent evt)
			{
				if( evt.getClickCount() >= 2)
				{
					editCell( evt.getPoint());
				}
			}
		});
	}

	public void editCell( Point p)
	{
		log.debug( "Editing cell at " + p);

		int columnIndex = columnAtPoint( p);
		int rowIndex = rowAtPoint( p);
		if( rowIndex >= 0 && columnIndex == 1)
		{
			log.debug( "Editing color: " + columnIndex);
		}
	}

}
