package com.cms.designer.util;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.Window;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class Utilities
{

	public Utilities()
	{
		super();
	}

	public static void center(Window frame)
	{
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension frameSize = frame.getSize();
		frame.setLocation(
			screenSize.width / 2 - (frameSize.width / 2),
			screenSize.height / 2 - (frameSize.height / 2));
	}
}
