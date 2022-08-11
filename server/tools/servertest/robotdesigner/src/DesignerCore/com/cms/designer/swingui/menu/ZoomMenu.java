package com.cms.designer.swingui.menu;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Scale;

public class ZoomMenu extends JMenu
{
	private OBEDesigner parent = null;

	private ButtonGroup group;

	public ZoomMenu( OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}

	private void init()
	{
		group = new ButtonGroup();
		for( int i = 0; i < Scale.DEFAULT_SET.length; i++)
		{
			final Scale scale = Scale.DEFAULT_SET[i];
			JCheckBoxMenuItem menuItem = new JCheckBoxMenuItem( scale.toString());
			menuItem.addActionListener( new ActionListener()
			{
				public void actionPerformed( ActionEvent evt)
				{
					parent.setScale( scale);
				}
			});
			if( scale.equals( new Scale( 1.0)))
			{
				menuItem.setSelected( true);
			}
			add( menuItem);
			group.add( menuItem);
		}

	}
}
