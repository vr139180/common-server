package com.cms.designer.swingui.renderer;

import javax.swing.Icon;

public class IconListItem
{
	private Icon icon;
	private String text;
	private String uuid;
	
	public IconListItem(Icon icon, String text)
	{
		this.icon = icon;
		this.text = text;
	}
	
	public IconListItem(Icon icon, String text, String uuid)
	{
		this( icon, text);
		this.uuid =uuid;
	}
	
	public Icon getIcon()
	{
		return icon;
	}
	
	public String getText()
	{
		return text;
	}
	
	public void setIcon(Icon icon)
	{
		this.icon = icon;
	}
	
	public void setText(String text)
	{
		this.text = text;
	}
	/**
	 * @return
	 */
	public String getUuid()
	{
		return uuid;
	}

	/**
	 * @param uuid
	 */
	public void setUuid(String uuid)
	{
		this.uuid = uuid;
	}

}
