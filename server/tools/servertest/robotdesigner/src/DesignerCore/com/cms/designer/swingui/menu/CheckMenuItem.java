package com.cms.designer.swingui.menu;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBoxMenuItem;

import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.swingui.OBEMenuBar;

public class CheckMenuItem extends JCheckBoxMenuItem
{
	private OBEMenuBar menuBar;

	private OBEInternalFrame inFrame;

	private String name;

	public CheckMenuItem( OBEInternalFrame inFrame)
	{
		this.inFrame = inFrame;
		menuBar = inFrame.getOBEMenuBar();
		this.name = inFrame.getName();
		this.setName( name);
		init();
	}

	public void init()
	{
		this.setText( name);
		this.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent e)
			{
				menuBar.getDesignerParent().getDesktopPanel().showFrame( name);
			}
		});
	}

	public void close()
	{
		menuBar.getDesignerParent().getCheckMenuGroup().remove( this);
		menuBar.getWindowMenu().remove( this);
	}
}