package com.cms.designer;

import java.net.URL;
import java.net.URLClassLoader;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class Main
{
	public static void main(String[] args) throws Exception
	{
		URL []url =new URL[0];
		URLClassLoader classLoader =new URLClassLoader( url);
		Class css =classLoader.loadClass( "com.cms.designer.JDesigner");
		css.newInstance();
	}
}
