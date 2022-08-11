package com.cms.designer.swingui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.border.Border;

import com.cms.core.util.ResourceUtil;

public class ColorComboBox extends JComboBox
{
	private static Object[] colorLists = new Object[]{
			new Object[]{Color.black, ResourceUtil.getRS( "color.black")},
			new Object[]{Color.blue, ResourceUtil.getRS( "color.blue")},
			new Object[]{Color.gray, ResourceUtil.getRS( "color.gray")},
			new Object[]{Color.green, ResourceUtil.getRS( "color.green")},
			new Object[]{Color.magenta, ResourceUtil.getRS( "color.magenta")},
			new Object[]{Color.orange, ResourceUtil.getRS( "color.orange")},
			new Object[]{Color.red, ResourceUtil.getRS( "color.red")},
			new Object[]{Color.yellow, ResourceUtil.getRS( "color.yellow")}};

	public ColorComboBox()
	{
		super( colorLists);
		init();
	}

	private void init()
	{
		ListCellRenderer renderer = new ColorListCellRenderer();
		setRenderer( renderer);
	}

	class ColorListCellRenderer extends JLabel implements ListCellRenderer
	{
		private ColorIcon icon = new ColorIcon();

		private Border lineBorder = BorderFactory.createLineBorder( Color.red, 2);

		private Border emptyBorder = BorderFactory.createEmptyBorder( 2, 2, 2, 2);

		public ColorListCellRenderer()
		{
			setOpaque( true);
		}

		public Component getListCellRendererComponent( JList list, Object value, int index,
				boolean isSelected, boolean cellHasFocus)
		{
			Object[] array = (Object[]) value;
			icon.setColor( (Color) array[0]);
			setIcon( icon);
			setText( (String) array[1]);

			if( isSelected)
			{
				setBorder( lineBorder);
				setForeground( list.getSelectionForeground());
				setBackground( list.getSelectionBackground());
			}
			else
			{
				setBorder( emptyBorder);
				setForeground( list.getForeground());
				setBackground( list.getBackground());
			}

			return this;
		}

		public ColorIcon getColorIcon()
		{
			return icon;
		}
	}

	class ColorIcon implements Icon
	{
		private Color color;

		private int w, h;

		public ColorIcon()
		{
			this( Color.gray, 50, 15);
		}

		public ColorIcon( Color color, int w, int h)
		{
			this.color = color;
			this.w = w;
			this.h = h;
		}

		public void paintIcon( Component c, Graphics g, int x, int y)
		{
			g.setColor( Color.black);
			g.drawRect( x, y, w - 1, h - 1);
			g.setColor( color);
			g.fillRect( x + 1, y + 1, w - 2, h - 2);
		}

		public Color getColor()
		{
			return color;
		}

		public void setColor( Color color)
		{
			this.color = color;
		}

		public int getIconWidth()
		{
			return w;
		}

		public int getIconHeight()
		{
			return h;
		}
	}

	public void setSelectedColor( Color color)
	{
		for( int i = 0; i < this.getItemCount(); i++)
		{
			Object[] array = (Object[]) this.getItemAt( i);
			if( array[0].equals( color))
			{
				setSelectedIndex( i);
				break;
			}
		}
	}

	public Color getColor( int index)
	{
		Object[] array = (Object[]) getItemAt( index);
		return (Color) array[0];
	}

	public static String getColorName( int index)
	{
		Object[] array = (Object[]) colorLists[index];
		return (String) array[1];
	}

	public static Color getColor( String name)
	{
		Color color = null;
		for( int i = 0; i < colorLists.length; i++)
		{
			Object[] array = (Object[]) colorLists[i];
			if( array[1].equals( name))
			{
				color = (Color) array[0];
				break;
			}
		}
		return color;
	}

	public static String getName( Color color)
	{
		String name = null;
		for( int i = 0; i < colorLists.length; i++)
		{
			Object[] array = (Object[]) colorLists[i];
			if( array[0].equals( color))
			{
				name = (String) array[1];
				break;
			}
		}
		return name;
	}

}