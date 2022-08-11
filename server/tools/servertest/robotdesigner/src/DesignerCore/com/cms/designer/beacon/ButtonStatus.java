package com.cms.designer.beacon;

import javax.swing.AbstractButton;

/**
 * @author excenon $Id: ButtonStatus.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 *         $Id: ButtonStatus.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class ButtonStatus
{
	private String name;

	private AbstractButton button;

	private int status;

	public ButtonStatus( String name, AbstractButton button, int status)
	{
		this.name = name;
		this.button = button;
		this.status = status;
	}

	/**
	 * @return Returns the button.
	 */
	public AbstractButton getButton()
	{
		return button;
	}

	/**
	 * @return Returns the status.
	 */
	public int getStatus()
	{
		return status;
	}

	/**
	 * @return Returns the name.
	 */
	public String getName()
	{
		return name;
	}
}
