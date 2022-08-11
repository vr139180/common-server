package com.cms.designer.swingui;

import java.awt.Insets;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.JToolBar;

/**
 * @author zagio
 *
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */
public class OBEComponentBar extends OBEToolBar
{
	private static Insets NO_MARGIN = new Insets(0, 0, 0, 0);
	private String eventType;
	//´æ·Å°´Å¥
	private Map hm;

	public OBEComponentBar(OBEDesigner parent)
	{
		super(parent);
		
		eventType = "";
		
		setBorderPainted(true);
		setRollover(true);
		setOrientation(JToolBar.VERTICAL);
		setMargin( NO_MARGIN);
		
		hm = new HashMap();
	}

	public Map getHm()
	{
		return hm;
	}
	
	public OBEComponentBar getToolbar()
	{
		if( hm.size() == 0)
			return null;
		else
			return this;
	}
	
	public boolean containButton( String name)
	{
		return hm.containsKey( name.toLowerCase());
	}
	
	public JButton getButton( String name)
	{
		return ( JButton)hm.get( name.toLowerCase());
	}
	
	public void addButton( String name, JButton jb)
	{
		hm.put( name.toLowerCase(), jb);
		add( jb);
	}
	
	public void modifyBorder(String key)
	{
		eventType = key;
		JButton b = (JButton)hm.get(key);
		b.setSelected(true);
		Iterator it = hm.values().iterator();
		while (it.hasNext())
		{
			JButton otherB = (JButton)it.next();
			if (otherB != b)
				otherB.setSelected(false);
		}
	}

	public String getEventType()
	{
		return eventType;
	}
	
	public void setEventType(String eventType)
	{
		this.eventType = eventType;
	}
}
