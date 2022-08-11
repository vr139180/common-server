package com.cms.designer.workflow.event;

import java.awt.Cursor;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.ImageIcon;

import com.anthonyeden.lib.util.IconManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.util.EditMode;
import com.cms.designer.util.ExtendedAttributeNames;
import com.cms.designer.util.ResourceUtil;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.view.ActivityView;

public class CreateTargetListener
	extends MouseAdapter
	implements MouseListener, MouseMotionListener, ExtendedAttributeNames
{
	public static IconManager iconManager = ResourceUtil.getIconManager();

	private ImageIcon normalIcon =
		(ImageIcon)iconManager.getIcon("targetCursor.gif");
	private ImageIcon innerIcon =
		(ImageIcon)iconManager.getIcon("targetInnerCursor.gif");

	private Cursor normalCursor =
		Toolkit.getDefaultToolkit().createCustomCursor(
			normalIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor innerCursor =
		Toolkit.getDefaultToolkit().createCustomCursor(
			innerIcon.getImage(),
			new java.awt.Point(0, 0),
			"");

	private OBEGraphWorkflow graph;
	private OBEModuleStandard module;
	
	private Point fromPoint, currentPoint;
	private ActivityView fromView;
	private String eventType;

	public CreateTargetListener( OBEGraphWorkflow graph, OBEModuleStandard module)
	{
		this.graph = graph;
		this.module =module;
	}

	public void mouseClicked(MouseEvent e)
	{
	}

	public void mousePressed(MouseEvent e)
	{
		eventType =module.getModuleToolbar().getEventType();
		if( isCreateTagetType(eventType))
		{
			if (e.getButton() == MouseEvent.BUTTON1)
			{
				eventType =module.getModuleToolbar().getEventType();
				graph.setLastLocation(e.getPoint());
				graph.addComponent(eventType);
			}
		}
	}

	public void mouseReleased(MouseEvent e)
	{
		eventType =module.getModuleToolbar().getEventType();
		if( isCreateTagetType(eventType))
		{
			if (e.getButton() == MouseEvent.BUTTON3)
			{
				module.getModuleToolbar().setEventType( OBEWorkflowModule.EDIT);
				module.getModuleToolbar().modifyBorder( OBEWorkflowModule.EDIT);
				graph.getDesignerParent().setEditMode(EditMode.EDIT);
			}
		}
	}

	public void mouseDragged(MouseEvent e)
	{
	}

	public void mouseMoved(MouseEvent e)
	{
		eventType =module.getModuleToolbar().getEventType();
		setMouseCursor( eventType);
		graph.setCurrentPoint( e.getPoint());
	}
	
	/**
	 * 设置鼠标类型
	 * @param type
	 */
	private void setMouseCursor( String type)
	{
		if( type == null)
		{
			graph.setCursor( null);
			return;
		}

		if( eventType.equals( OBEWorkflowModule.EDIT))
			graph.setCursor( null);
		else if( eventType.equals( OBEWorkflowModule.TOOLIMP))
			graph.setCursor( normalCursor);
	}

	private boolean isCreateTagetType(String eventType)
	{
		boolean isCreateTaget = false;
		if (eventType.equals( OBEWorkflowModule.TOOLIMP))
		{
			isCreateTaget = true;
		}
		
		return isCreateTaget;
	}

}