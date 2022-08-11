package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 * $Id: EditSelectNone.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class EditSelectNone extends OBEStandardAction
{
	private String key =null;
	public EditSelectNone( OBEDesigner parent)
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
