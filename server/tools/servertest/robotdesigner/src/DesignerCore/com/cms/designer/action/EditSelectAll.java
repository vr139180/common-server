package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * $Id: EditSelectAll.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class EditSelectAll extends OBEStandardAction
{
	private String key =null;
	public EditSelectAll( OBEDesigner parent)
	{
		super(parent);
	}
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		return;
	}
	/**
	 * @return
	 */
	public String getKey()
	{
		return key;
	}

	/**
	 * @param key
	 */
	public void setKey(String key)
	{
		this.key = key;
	}
}
