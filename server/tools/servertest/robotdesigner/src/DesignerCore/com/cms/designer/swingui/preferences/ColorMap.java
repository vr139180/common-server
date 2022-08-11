package com.cms.designer.swingui.preferences;

import java.awt.Color;

public class ColorMap
{
	private String name;

	private Color color;

	public ColorMap( String name, Color color)
	{
		setName( name);
		setColor( color);
	}

	public ColorMap( String name, String color)
	{
		setName( name);
		setColor( color);
	}

	public String getName()
	{
		return name;
	}

	public void setName( String name)
	{
		this.name = name;
	}

	public Color getColor()
	{
		return color;
	}

	public void setColor( Color color)
	{
		this.color = color;
	}

	public void setColor( String color)
	{
		setColor( Color.decode( color));
	}

}
