package com.cms.designer.swingui.ui;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.FlowLayout;

import javax.swing.JPanel;
import javax.swing.JToolBar;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class DocingPanelManager
{
	private OBEDesigner designer =null;
	private JPanel	calcuatePanel =null;
	
	private JPanel	northPanel, southPanel, eastPanel, westPanel;
	
	public DocingPanelManager( OBEDesigner designer, JPanel ppanel)
	{
		this.designer =designer;
		calcuatePanel =ppanel;
		
		init();
	}
	
	public void addOneToolBar( JToolBar t, String or)
	{
		if( or.equals( BorderLayout.NORTH))
			northPanel.add( t);
		else if( or.equals( BorderLayout.SOUTH))
			southPanel.add( t);
		else if( or.equals( BorderLayout.WEST))
			westPanel.add( t);
		else if( or.equals( BorderLayout.EAST))
			eastPanel.add( t);
	}
	
	public void validate( String s)
	{
		if( s.equals( BorderLayout.NORTH))
			northPanel.validate();
		else if( s.equals( BorderLayout.SOUTH))
			southPanel.validate( );
		else if( s.equals( BorderLayout.WEST))
			westPanel.validate( );
		else if( s.equals( BorderLayout.EAST))
			eastPanel.validate( );
	}

	public void invalidate( String s)
	{
		if( s.equals( BorderLayout.NORTH))
			northPanel.invalidate();
		else if( s.equals( BorderLayout.SOUTH))
			southPanel.invalidate( );
		else if( s.equals( BorderLayout.WEST))
			westPanel.invalidate( );
		else if( s.equals( BorderLayout.EAST))
			eastPanel.invalidate( );
	}
	
	public void updateUI()
	{
		northPanel.updateUI();
		southPanel.updateUI();
		westPanel.updateUI();
		eastPanel.updateUI();
	}
	
	public String removeOneToolBar( JToolBar t)
	{
		String ret =null;
		Component[] comp =northPanel.getComponents();
		for( int i =0; i<comp.length; ++i)
		{
			if( comp[i] == t)
			{
				northPanel.remove( t);
				return BorderLayout.NORTH;
			}
		}

		comp =southPanel.getComponents();
		for( int i =0; i<comp.length; ++i)
		{
			if( comp[i] == t)
			{
				southPanel.remove( t);
				return BorderLayout.SOUTH;
			}
		}

		comp =westPanel.getComponents();
		for( int i =0; i<comp.length; ++i)
		{
			if( comp[i] == t)
			{
				westPanel.remove( t);
				return BorderLayout.WEST;
			}
		}

		comp =eastPanel.getComponents();
		for( int i =0; i<comp.length; ++i)
		{
			if( comp[i] == t)
			{
				eastPanel.remove( t);
				return BorderLayout.EAST;
			}
		}
		
		return ret;
	}
	
	public void removeOneToolBar( JToolBar t, String or)
	{
		if( or.equals( BorderLayout.NORTH))
			northPanel.remove( t);
		else if( or.equals( BorderLayout.SOUTH))
			southPanel.remove( t);
		else if( or.equals( BorderLayout.WEST))
			westPanel.remove( t);
		else if( or.equals( BorderLayout.EAST))
			eastPanel.remove( t);
	}

	public void removeAllToolBars( String or)
	{
		if( or.equals( BorderLayout.NORTH))
			northPanel.removeAll();
		else if( or.equals( BorderLayout.SOUTH))
			southPanel.removeAll( );
		else if( or.equals( BorderLayout.WEST))
			westPanel.removeAll( );
		else if( or.equals( BorderLayout.EAST))
			eastPanel.removeAll( );
	}
	
	public Container getDocingSource()
	{
		return calcuatePanel;
	}
	
	private void init()
	{
		northPanel =new JPanel();
		northPanel.setLayout( new FlowLayout( FlowLayout.LEFT));
		calcuatePanel.add( northPanel, BorderLayout.NORTH);

		southPanel =new JPanel();
		southPanel.setLayout( new FlowLayout( FlowLayout.LEFT));
		calcuatePanel.add( southPanel, BorderLayout.SOUTH);

		eastPanel =new JPanel();
		eastPanel.setLayout( new FlowLayout( FlowLayout.LEFT));
		calcuatePanel.add( eastPanel, BorderLayout.EAST);

		westPanel =new JPanel();
		westPanel.setLayout( new FlowLayout( FlowLayout.LEFT));
		calcuatePanel.add( westPanel, BorderLayout.WEST);
	}
	
}
