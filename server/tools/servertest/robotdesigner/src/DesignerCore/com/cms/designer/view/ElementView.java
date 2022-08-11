package com.cms.designer.view;

import com.cms.core.BasicElement;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class ElementView
{
	private BasicElement element;
	private Object cell;

	public ElementView()
	{}

	public Object getCell()
	{
		return cell;
	}

	public void setCell(Object cell)
	{
		this.cell = cell;
	}

	/**
	 * @return
	 */
	public BasicElement getElement()
	{
		return element;
	}

	/**
	 * @param element
	 */
	public void setElement(BasicElement element)
	{
		this.element = element;
	}

}
