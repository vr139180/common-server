package com.cms.designer.swingui.menu;

import java.util.Map;

import javax.swing.Action;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEGraphWorkflow;

public class OBEGraphPopup extends JPopupMenu
{
	private OBEDesigner app;

	private OBEGraphWorkflow parent;

	private ZoomMenu zoomMenu;

	private JMenuItem addActivityMenuItem;

	private JMenuItem addTransitionMenuItem;

	private JMenuItem editMenuItem;

	private JMenuItem deleteMenuItem;

	private Object object;

	public OBEGraphPopup( OBEDesigner app, OBEGraphWorkflow parent)
	{
		this( app, parent, null);
	}

	public OBEGraphPopup( OBEDesigner app, OBEGraphWorkflow parent, Object object)
	{
		this.app = app;
		this.parent = parent;
		this.object = object;
		init();
	}

	public void loadResources()
	{
		zoomMenu.setText( "Zoom");

		if( addActivityMenuItem != null)
		{
			addActivityMenuItem.setText( "Add Activity...");
		}

		if( addTransitionMenuItem != null)
		{
			addTransitionMenuItem.setText( "Add Transition...");
		}

		if( editMenuItem != null)
		{
			editMenuItem.setText( "Edit...");
		}

		if( deleteMenuItem != null)
		{
			deleteMenuItem.setText( "Delete");
		}
	}

	private void init()
	{
		boolean needsSeparator = true;

		Map actions = parent.getActions();

		zoomMenu = new ZoomMenu( app);
		add( zoomMenu);

		if( canAddActivity( object))
		{
			if( needsSeparator)
			{
				addSeparator();
				needsSeparator = false;
			}
			addActivityMenuItem = add( (Action) actions.get( "graph.addActivity"));
		}

		if( canAddTransition( object))
		{
			if( needsSeparator)
			{
				addSeparator();
				needsSeparator = false;
			}
			addTransitionMenuItem = add( (Action) actions.get( "graph.addTransition"));
		}

		if( canEdit( object))
		{
			addSeparator();
			editMenuItem = add( (Action) actions.get( "graph.edit"));
		}

		if( canDelete( object))
		{
			addSeparator();
			deleteMenuItem = add( (Action) actions.get( "graph.delete"));
		}

		loadResources();
	}

	private boolean canAddActivity( Object object)
	{
		/*
		 * ObjectDescriptor od = getObjectDescriptor(object); if(od != null){
		 * if( od instanceof TaskDescriptor ){ return true; } } return false;
		 */
		return true;
	}

	private boolean canAddTransition( Object object)
	{
		/*
		 * ObjectDescriptor od = getObjectDescriptor(object); if(od != null){
		 * if( od instanceof TaskDescriptor ){ return true; } } return false;
		 */
		return true;
	}

	private boolean canEdit( Object object)
	{
		/*
		 * ObjectDescriptor od = getObjectDescriptor(object); if(od != null){
		 * if( od instanceof TaskDescriptor || od instanceof
		 * ConditionDescriptor){ return true; } } return false;
		 */
		return true;
	}

	private boolean canDelete( Object object)
	{
		/*
		 * ObjectDescriptor od = getObjectDescriptor(object); if(od != null){
		 * if( od instanceof TaskDescriptor || od instanceof
		 * ConditionDescriptor || od instanceof StopPointDescriptor){ return
		 * true; } } return false;
		 */
		return true;
	}

}
