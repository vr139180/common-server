package com.cms.designer;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.LogoWindow;
import com.cms.designer.swingui.OBEDesigner;

/**
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class JDesigner
{
	private OBEDesigner app =null;

	public JDesigner()
	{
		//入口
		LogoWindow.showLogoWindow();
		//加载资源s
		ResourceUtil.init("com.cms.designer.resource");
		showDesignerMainFrame();
		LogoWindow.disposeLogoWindow();
	}
	
	private void showDesignerMainFrame()
	{
		if( app == null)
			app = new OBEDesigner();
		app.setVisible(true);
	}
}
