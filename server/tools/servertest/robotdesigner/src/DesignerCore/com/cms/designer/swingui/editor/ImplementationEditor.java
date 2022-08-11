package com.cms.designer.swingui.editor;

import java.awt.Component;

import com.cms.core.workflow.Implementation;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public interface ImplementationEditor
{

	public String getTitle();

	public Component getComponent();

	public Implementation getImplementation();

	public void setImplementation(Implementation implementation);

	public boolean save();

	public void revert();
}
