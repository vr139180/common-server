package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Icon;

import com.cms.designer.swingui.OBEDesigner;

public class OBEStandardAction extends AbstractAction
{
	protected OBEDesigner parent;

	public OBEStandardAction(OBEDesigner parent)
	{
		super();
		this.parent = parent;
	}

	public OBEStandardAction(OBEDesigner parent, String s)
	{
		super(s);
		this.parent = parent;
	}

	public OBEStandardAction(OBEDesigner parent, String s, Icon icon)
	{
		super(s, icon);
		this.parent = parent;
	}

	public void actionPerformed(ActionEvent e)
	{
		/**@todo Implement this java.awt.event.ActionListener abstract method*/
	}
	/*
	public void setSelectionAttributes(Map map)
	{
		parent.getAgentGraph().setSelectionAttributes(map);
	}
	*/
	
	/**
	 * @return
	 */
	public OBEDesigner getParent()
	{
		return parent;
	}

}