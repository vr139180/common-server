package com.cms.designer.swingui.renderer;

import java.awt.Color;
import java.awt.Component;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.border.Border;

public class IconListItemRenderer extends JLabel implements ListCellRenderer
{
	private Border selectedBorder = BorderFactory.createLineBorder( Color.red, 1),
			emptyBorder = BorderFactory.createEmptyBorder( 1, 1, 1, 1);

	public IconListItemRenderer()
	{
		setOpaque( true);
	}

	public Component getListCellRendererComponent( JList list, Object value, int index,
			boolean isSelected, boolean cellHasFocus)
	{
		//if ( ! (value instanceof IconListItem) ) return null;
		IconListItem item = (IconListItem) value;
		if( isSelected)
		{
			setBorder( selectedBorder);
			setForeground( list.getSelectionForeground());
			setBackground( list.getSelectionBackground());
		}
		else
		{
			setBorder( emptyBorder);
			setForeground( list.getForeground());
			setBackground( list.getBackground());
		}
		this.setIcon( item.getIcon());
		this.setText( item.getText());
		repaint();
		return this;
	}

}
