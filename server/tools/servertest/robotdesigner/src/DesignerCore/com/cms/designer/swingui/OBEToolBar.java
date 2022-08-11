package com.cms.designer.swingui;

import java.awt.Dimension;
import java.awt.Insets;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JToolBar;

import com.cms.designer.swingui.ui.OBEToolBarUI;

public class OBEToolBar extends JToolBar
{
	private static Insets NO_MARGIN = new Insets(0, 0, 0, 0);
	private static Dimension size = new Dimension(30, 30);
	private List buttonNames =null;

	private OBEDesigner parent =null;

	public OBEToolBar(OBEDesigner parent)
	{
		this.parent = parent;
		this.setName("excenon toolbar");

		init();
		buttonNames =new ArrayList();
	}

	public void init()
	{
		setUI( new OBEToolBarUI(parent));
		
		setBorderPainted( true);
		setRollover( true);
		setMargin( NO_MARGIN);
	}

	public OBEDesigner getOBEDesigner()
	{
		return parent;
	}
	
	public void addButton( String n)
	{
		buttonNames.add( n);
	}
	
	public List getButtons()
	{
		return buttonNames;
	}
	
}
