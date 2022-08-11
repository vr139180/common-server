package com.cms.designer.swingui.menu;

import java.awt.Point;
import java.util.Map;

import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEGraphWorkflow;

/**
 * @author administrator
 *
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 * $Id: OBEGraphEdgePopup.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class OBEGraphEdgePopup extends JPopupMenu
{
	private static IconManager iconManager = new IconManager("wf/");
	private static Icon tm = iconManager.getIcon("tm.gif");
	private OBEDesigner designer;
	private OBEGraphWorkflow parent;
	private Object object;
	private Point mousePoint;

	private JMenuItem snapSingleMenuItem;
	private JMenuItem snapFree;
	private JMenuItem toLineMenuItem;
	private JMenuItem toBowMenuItem;

	private JMenuItem snapHorizontalMenuItem;
	private JMenuItem snapVerticalMenuItem;
	private JMenuItem snapLeftMenuItem;
	private JMenuItem snapRightMenuItem;

	private JMenuItem propertyMenuItem;
	private JMenuItem reverseMenuItem;

	private JMenuItem deleteMenuItem;
	
	public OBEGraphEdgePopup(
		OBEDesigner designer,
		OBEGraphWorkflow graph,
		Object object,
		Point mousePoint)
	{
		this.designer = designer;
		this.parent = graph;
		this.object = object;
		this.mousePoint = mousePoint;
		init();
	}

	public void loadResources()
	{
		if (propertyMenuItem != null)
		{
			propertyMenuItem.setText(
				ResourceUtil.getRS("properties.properties"));
			propertyMenuItem.setIcon(iconManager.getIcon("properties.gif"));

		}
		if (reverseMenuItem != null)
		{
			reverseMenuItem.setText(ResourceUtil.getRS("transition.reverse"));
			reverseMenuItem.setIcon(tm);
		}
		if (toLineMenuItem != null)
		{
			toLineMenuItem.setText(ResourceUtil.getRS("transition.toLine"));
			toLineMenuItem.setIcon(tm);
		}

		if (toBowMenuItem != null)
		{
			toBowMenuItem.setText(ResourceUtil.getRS("transition.toBow"));
			toBowMenuItem.setIcon(tm);
		}

		if (deleteMenuItem != null)
		{
			deleteMenuItem.setText(ResourceUtil.getRS("activity.delete"));
			deleteMenuItem.setIcon(tm);
		}
		if (snapHorizontalMenuItem != null)
		{
			snapHorizontalMenuItem.setIcon(iconManager.getIcon("hz.gif"));
			snapHorizontalMenuItem.setText(
				ResourceUtil.getRS("transition.snapHorizontal"));
		}
		if (snapVerticalMenuItem != null)
		{
			snapVerticalMenuItem.setIcon(iconManager.getIcon("sz.gif"));
			snapVerticalMenuItem.setText(
				ResourceUtil.getRS("transition.snapVertical"));
		}
		if (snapLeftMenuItem != null)
		{
			snapLeftMenuItem.setIcon(iconManager.getIcon("zz.gif"));
			snapLeftMenuItem.setText(ResourceUtil.getRS("transition.snapLeft"));
		}
		if (snapRightMenuItem != null)
		{
			snapRightMenuItem.setIcon(iconManager.getIcon("yz.gif"));
			snapRightMenuItem.setText(
				ResourceUtil.getRS("transition.snapRight"));
		}
		if (snapFree != null)
		{
			snapFree.setText(ResourceUtil.getRS("transition.snapFree"));
			snapFree.setIcon(tm);
		}
		if (snapSingleMenuItem != null)
		{
			snapSingleMenuItem.setText(
				ResourceUtil.getRS("transition.snapSingle"));
			snapSingleMenuItem.setIcon(tm);
		}

	}

	private void init()
	{

		Map actions = parent.getActions();

		propertyMenuItem = add((Action)actions.get("graph.edit"));
		reverseMenuItem = add((Action)actions.get("graph.reverse"));
		addSeparator();
		if (canToLine(object))
		{
			toLineMenuItem = add((Action)actions.get("graph.toLine"));
		}

		if (canToBow(object))
		{
		}

		if (canSnap(object))
		{
			addSeparator();
			snapHorizontalMenuItem =
				add((Action)actions.get("graph.snapHorizontal"));
			snapVerticalMenuItem =
				add((Action)actions.get("graph.snapVertical"));
			snapLeftMenuItem = add((Action)actions.get("graph.snapLeft"));
			snapRightMenuItem = add((Action)actions.get("graph.snapRight"));
			addSeparator();
			snapSingleMenuItem = add((Action)actions.get("graph.snapSingle"));
		}

		if (canDelete(object))
		{
			addSeparator();
			deleteMenuItem = add((Action)actions.get("graph.delete"));
		}

		loadResources();
	}

	private boolean canToLine(Object object)
	{
		return object != null;
	}

	private boolean canToBow(Object object)
	{
		return object != null;
	}

	private boolean canEdit(Object object)
	{
		return object != null;
	}

	private boolean canDelete(Object object)
	{
		return object != null;
	}
	private boolean canSnap(Object object)
	{
		return object != null;
	}
}
