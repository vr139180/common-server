package com.cms.designer.swingui;

import javax.swing.JEditorPane;

import com.cms.designer.validate.Validate;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBECheckPanel.java,v 1.2 2004/08/19 08:49:08 york Exp $
 */
public class OBECheckPanel extends JEditorPane
{
	public OBECheckPanel()
	{
		super();
		
		init();
		
	}
	
	private void init()
	{
		this.setAutoscrolls( true);
		this.setEditable( false);
		this.setContentType( "text/html;charset=UTF-8");
	}
	
	public void showToolTip( Validate v)
	{
		String info =v.getFormatMsg();
		this.setText( info);
	}
}
