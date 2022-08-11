package com.cms.designer.swingui.menu;

import java.util.Map;

import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.graph.OBEEdge;
import com.cms.designer.workflow.graph.OBEGraphCell;
import com.cms.designer.workflow.menu.AddActivityMenu;
import com.cms.designer.workflow.view.ActivityView;

public class OBEGraphActivityModePopup extends JPopupMenu
{
	private static IconManager iconManager = new IconManager( "wf/");

	private static Icon tm = iconManager.getIcon( "tm.gif");

	private OBEDesigner app;

	private OBEGraphWorkflow parent;

	private Object object;

	private ZoomMenu zoomMenu;

	private AddActivityMenu addActivityMenu;

	//ɾ��
	private JMenuItem deleteMenuItem;

	//��������
	private JMenuItem propertyMenuItem;

	//ʵ�ʴ�С
	private JMenuItem actualSizeMenuItem;

	//�Ŵ�
	private JMenuItem zoomInMenuItem;

	//��С
	private JMenuItem zoomOutMenuItem;

	public OBEGraphActivityModePopup( OBEDesigner app, OBEGraphWorkflow parent, Object object)
	{
		this.app = app;
		this.parent = parent;
		this.object = object;

		if( object != null && object instanceof OBEEdge)
			return;

		init();
	}

	public void loadResources()
	{
		propertyMenuItem.setText( ResourceUtil.getRS( "activity.property"));
		propertyMenuItem.setIcon( iconManager.getIcon( "properties.gif"));

		if( zoomMenu != null)
			zoomMenu.setText( ResourceUtil.getRS( "window.zoom"));

		if( zoomInMenuItem != null)
		{
			zoomInMenuItem.setText( ResourceUtil.getRS( "popup.zoomIn"));
			zoomInMenuItem.setIcon( iconManager.getIcon( "zoomIn1.gif"));
		}
		if( zoomOutMenuItem != null)
		{
			zoomOutMenuItem.setText( ResourceUtil.getRS( "popup.zoomOut"));
			zoomOutMenuItem.setIcon( iconManager.getIcon( "zoomOut1.gif"));
		}
		if( actualSizeMenuItem != null)
		{
			actualSizeMenuItem.setIcon( iconManager.getIcon( "actualSize.gif"));
			actualSizeMenuItem.setText( ResourceUtil.getRS( "popup.actualSize"));
		}

		if( addActivityMenu != null)
			addActivityMenu.setText( ResourceUtil.getRS( "activity.newActivity"));

		if( deleteMenuItem != null)
			deleteMenuItem.setText( ResourceUtil.getRS( "activity.delete"));
	}

	/**
	 * ��ʼ���˵�
	 */
	private void init()
	{
		Map actions = parent.getActions();

		propertyMenuItem = add( (Action) actions.get( "graph.edit"));

		if( object == null)
		{
			//���ӷŴ���С�˵�
			addSeparator();
			actualSizeMenuItem = add( (Action) actions.get( "graph.actualSize"));
			zoomInMenuItem = add( (Action) actions.get( "graph.zoomIn"));
			zoomOutMenuItem = add( (Action) actions.get( "graph.zoomOut"));
//			addSeparator();

			//���Ӵ����ڵ�˵�
//			addActivityMenu = new AddActivityMenu( app, parent);
//			add( addActivityMenu);
		}
		else
		{
			if( canDeleteEnabled( object))
			{
				addSeparator();
				deleteMenuItem = add( (Action) actions.get( "graph.delete"));
			}
		}

		loadResources();
	}

	private boolean CurIsAuto()
	{
		return parent.isAutoOPTEdge();
	}

	protected boolean canDeleteEnabled( Object object)
	{
		if( object == null)
			return false;

		if( object instanceof OBEGraphCell)
		{
			OBEGraphCell cell = (OBEGraphCell) object;
			ActivityView av =( ActivityView)cell.getUserObject();
			if( av != null)
			{
				Activity act =av.getActivity();
				if( act != null && act.getActivityType() != Activity.START)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
}
