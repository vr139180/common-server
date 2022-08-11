package com.cms.designer.workflow.event;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.ArrayList;

import javax.swing.ImageIcon;

import org.jgraph.graph.DefaultGraphCell;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.util.EditMode;
import com.cms.designer.util.ExtendedAttributeNames;
import com.cms.designer.view.ElementView;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.graph.OBEGraphCell;
import com.cms.designer.workflow.view.ActivityView;

/**
 * @author Administrator
 * 
 * To change the template for this generated type comment go to Window -
 * Preferences - Java - Code Generation - Code and Comments
 */
public class CreateLineTransitionListener extends MouseAdapter
		implements
			MouseListener,
			MouseMotionListener,
			ExtendedAttributeNames
{
	public static IconManager iconManager = new IconManager( "wf/");
	private static ImageIcon normalIcon = (ImageIcon) iconManager.getIcon( "transitionCursor.gif");
	private static ImageIcon startIcon = (ImageIcon) iconManager.getIcon( "startTransitionCursor.gif");
	private static ImageIcon endIcon = (ImageIcon) iconManager.getIcon( "endTransitionCursor.gif");
	private static ImageIcon stopIcon = (ImageIcon) iconManager.getIcon( "stop3.gif");
	private static Cursor normal = Toolkit.getDefaultToolkit().createCustomCursor( 
			normalIcon.getImage(), new java.awt.Point( 0, 0), "");
	private static Cursor startCursor = Toolkit.getDefaultToolkit().createCustomCursor( 
			startIcon.getImage(), new java.awt.Point( 0, 0), "");
	private static Cursor endCursor = Toolkit.getDefaultToolkit().createCustomCursor( 
			endIcon.getImage(), new java.awt.Point( 0, 0), "");
	private static Cursor stopCursor = Toolkit.getDefaultToolkit().createCustomCursor( 
			stopIcon.getImage(), new java.awt.Point( 0, 0), "");

	private OBEGraphWorkflow graph;

	private OBEModuleStandard module;

	private Point fromPoint, currentPoint;

	private ElementView fromView;

	private OBEGraphCell fromCell, toCell;

	private String eventType;

	public CreateLineTransitionListener( OBEGraphWorkflow graph, OBEModuleStandard module)
	{
		this.graph = graph;
		this.module = module;
	}

	public void mousePressed( MouseEvent e)
	{
		eventType = module.getModuleToolbar().getEventType();
		if( eventType.equals( OBEWorkflowModule.LINE))
		{
			if( e.getButton() == MouseEvent.BUTTON1)
			{
				Point p = e.getPoint();
				Object cell = graph.getFirstCellForLocation( p.x, p.y);
				if( fromPoint == null)
				{
					if( cell != null && cell instanceof OBEGraphCell)
					{
						fromCell = (OBEGraphCell) cell;
						Object userObject = fromCell.getUserObject();
						if( userObject instanceof ElementView)
						{
							fromView = (ElementView) userObject;
							fromPoint = e.getPoint();
						}
						else if( userObject instanceof String)
						{
							fromPoint = e.getPoint();
						}
					}
				}
				else
				{
					if( cell instanceof OBEGraphCell)
					{
						toCell = (OBEGraphCell) cell;
						Object userObject = toCell.getUserObject();
						if( userObject instanceof ElementView && fromView != null)
						{
							ElementView toView = (ElementView) userObject;
							if( !toView.equals( fromView))
							{
								ArrayList points = new ArrayList();
								if( eventType.equals( OBEWorkflowModule.LINE))
									graph.addTransition( fromView, toView, points);

								fromPoint = null;
								currentPoint = null;
								fromView = null;
							}
						}
					}
				}
			}
		}
	}

	public void mouseReleased( MouseEvent e)
	{
		eventType = module.getModuleToolbar().getEventType();
		if( eventType.equals( OBEWorkflowModule.LINE))
		{
			if( e.getButton() == MouseEvent.BUTTON3)
			{
				if( fromPoint != null)
				{
					fromPoint = null;
					currentPoint = null;
					fromView = null;
					graph.repaint();
				}
				else
				{
					module.getModuleToolbar().setEventType( OBEWorkflowModule.EDIT);
					module.getModuleToolbar().modifyBorder( OBEWorkflowModule.EDIT);
					graph.getDesignerParent().setEditMode( EditMode.EDIT);
				}
			}
		}
	}

	public void mouseDragged( MouseEvent e)
	{
	}

	public void mouseMoved( MouseEvent e)
	{
		eventType = module.getModuleToolbar().getEventType();
		if( !eventType.equals( OBEWorkflowModule.LINE))
			return;

		Point p = e.getPoint();
		Object obj = graph.getFirstCellForLocation( p.x, p.y);
		
		if( obj != null && obj instanceof OBEGraphCell)
		{
			OBEGraphCell cell = (OBEGraphCell) obj;
			if( cell.getUserObject() instanceof ElementView)
			{
				ElementView view = (ElementView) cell.getUserObject();

				if( fromPoint == null)
				{
					if( canAddFromTransition( view))
						graph.setCursor( startCursor);
					else
						graph.setCursor( stopCursor);
				}
				else
				{
					if( canAddToTransition( view))
						graph.setCursor( endCursor);
					else
						graph.setCursor( stopCursor);
				}
			}
		}
		else
		{
			graph.setCursor( normal);
		}

		if( fromPoint != null)
		{
			Point from = getCenterPoint( fromCell);
			currentPoint = p;
			Graphics g = graph.getGraphics();
			graph.repaint();
			paintConnector( g);
		}
	}

	private Point getCenterPoint( DefaultGraphCell cell)
	{
		Point from = null;
		Rectangle bounds = graph.getCellBounds( cell);

		double x = bounds.getCenterX();

		double y = bounds.getCenterY();
		from = new Point( (int) x, (int) y);
		return from;
	}

	private void paintConnector( Graphics g)
	{
		g.setColor( Color.black);
		g.setPaintMode();
		if( fromPoint != null && currentPoint != null)
		{
			g.drawLine( fromPoint.x, fromPoint.y, currentPoint.x, currentPoint.y);
		}
	}

	public void paint( Graphics g)
	{
		if( fromPoint != null)
		{
			paintConnector( g);
		}
	}

	private boolean canAddFromTransition( ElementView view)
	{
		boolean canAdd = true;
		if( view instanceof ActivityView)
		{
			ActivityView activityView = (ActivityView) view;
		}

		return canAdd;
	}

	private boolean canAddToTransition( ElementView view)
	{
		boolean canAdd = true;
		if( view instanceof ActivityView)
		{
			ActivityView activityView = (ActivityView) view;
			if( activityView.getActivity().getActivityType() == Activity.START)
				canAdd = false;
		}

		OBEGraphCell cell = (OBEGraphCell) view.getCell();
		if( cell == fromCell)
			canAdd = false;

		return canAdd;
	}

}
