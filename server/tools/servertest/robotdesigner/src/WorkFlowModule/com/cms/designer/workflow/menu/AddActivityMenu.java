package com.cms.designer.workflow.menu;

import java.util.Map;

import javax.swing.Action;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEGraphWorkflow;

public class AddActivityMenu extends JMenu
{
	private OBEDesigner app;
	private OBEGraphWorkflow parent;

	private JMenuItem toolSetMenuItem;
	private JMenuItem noImplementationMenuItem;

	public AddActivityMenu( OBEDesigner app, OBEGraphWorkflow parent)
	{
		this.app = app;
		this.parent = parent;
		init();
	}

	public void loadResources()
	{
		toolSetMenuItem.setText( ResourceUtil.getRS( "activity.tooSet"));
		noImplementationMenuItem.setText( ResourceUtil.getRS( "activity.noImplementation"));
	}

	private void init()
	{
		Map actions = parent.getActions();

		toolSetMenuItem = add( (Action) actions.get( "graph.addToolSet"));
		noImplementationMenuItem = add( (Action) actions.get( "graph.addNoImplementation"));

		loadResources();
	}

}
