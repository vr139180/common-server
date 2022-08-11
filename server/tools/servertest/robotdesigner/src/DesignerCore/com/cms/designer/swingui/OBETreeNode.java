package com.cms.designer.swingui;

import javax.swing.tree.DefaultMutableTreeNode;

/**
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */
public class OBETreeNode extends DefaultMutableTreeNode
{
	public final static String PACKAGE = "package";
	public final static String PROCESS = "process";
	public final static String WORKFLOW = "workflow";

	//系统固有属性
	public final static String ROOT = "wp.root";
	public final static String PROJECT = "wp.project";
	public final static String MODULE ="wp.extmodule";

	/**
	  * Constructor MyTreeNode.
	  * @param string
	  */
	public OBETreeNode( Object userObject)
	{
		super( userObject);
	}
}