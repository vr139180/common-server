package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;

/**
 * @author excenon
 * $Id: HelpTopic.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class HelpTopic extends OBEStandardAction
{

	/**
	 * @param parent
	 */
	public HelpTopic( OBEDesigner parent)
	{
		super( parent);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		
		try
		{
			Runtime.getRuntime().exec( "explorer.exe "+ResourceUtil.getRS("help.topicwww"));
		}
		catch( Exception ep)
		{
		}
		return;
	}
}
