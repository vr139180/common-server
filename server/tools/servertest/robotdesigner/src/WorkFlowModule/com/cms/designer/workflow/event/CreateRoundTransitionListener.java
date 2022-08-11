package com.cms.designer.workflow.event;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;

import javax.swing.ImageIcon;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jgraph.graph.DefaultGraphCell;

import com.anthonyeden.lib.util.IconManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.util.ExtendedAttributeNames;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.view.ActivityView;

public class CreateRoundTransitionListener
	extends MouseAdapter
	implements MouseMotionListener, ExtendedAttributeNames
{
	private static final Log log =
		LogFactory.getLog(CreateRoundTransitionListener.class);

	private OBEGraphWorkflow graph;
	private OBEModuleStandard module;
	
	private Point fromPoint, currentPoint;
	private ActivityView fromView;
	private String eventType;

	public static IconManager iconManager = new IconManager("wf/");

	private ImageIcon normalIcon =
		(ImageIcon)iconManager.getIcon("transitionCursor.gif");
	private Cursor normal =
		Toolkit.getDefaultToolkit().createCustomCursor(
			normalIcon.getImage(),
			new java.awt.Point(0, 0),
			"");

	private ImageIcon startCenterIcon =
		(ImageIcon)iconManager.getIcon("startCenter.gif");
	private ImageIcon startLeftIcon =
		(ImageIcon)iconManager.getIcon("startLeft.gif");
	private ImageIcon startRightIcon =
		(ImageIcon)iconManager.getIcon("startRight.gif");
	private ImageIcon startTopIcon =
		(ImageIcon)iconManager.getIcon("startTop.gif");
	private ImageIcon startBottomIcon =
		(ImageIcon)iconManager.getIcon("startBottom.gif");

	private ImageIcon endCenterIcon =
		(ImageIcon)iconManager.getIcon("endCenter.gif");
	private ImageIcon endLeftIcon =
		(ImageIcon)iconManager.getIcon("endLeft.gif");
	private ImageIcon endRightIcon =
		(ImageIcon)iconManager.getIcon("endRight.gif");
	private ImageIcon endTopIcon = (ImageIcon)iconManager.getIcon("endTop.gif");
	private ImageIcon endBottomIcon =
		(ImageIcon)iconManager.getIcon("endBottom.gif");

	private Cursor startCenter =
		Toolkit.getDefaultToolkit().createCustomCursor(
			startCenterIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor startLeft =
		Toolkit.getDefaultToolkit().createCustomCursor(
			startLeftIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor startRight =
		Toolkit.getDefaultToolkit().createCustomCursor(
			startRightIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor startTop =
		Toolkit.getDefaultToolkit().createCustomCursor(
			startTopIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor startBottom =
		Toolkit.getDefaultToolkit().createCustomCursor(
			startBottomIcon.getImage(),
			new java.awt.Point(0, 0),
			"");

	private Cursor endCenter =
		Toolkit.getDefaultToolkit().createCustomCursor(
			endCenterIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor endLeft =
		Toolkit.getDefaultToolkit().createCustomCursor(
			endLeftIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor endRight =
		Toolkit.getDefaultToolkit().createCustomCursor(
			endRightIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor endTop =
		Toolkit.getDefaultToolkit().createCustomCursor(
			endTopIcon.getImage(),
			new java.awt.Point(0, 0),
			"");
	private Cursor endBottom =
		Toolkit.getDefaultToolkit().createCustomCursor(
			endBottomIcon.getImage(),
			new java.awt.Point(0, 0),
			"");

	public CreateRoundTransitionListener( OBEGraphWorkflow graph, OBEModuleStandard module)
	{
		this.graph = graph;
		this.module =module;
	}

	public void mousePressed(MouseEvent e)
	{
		//		eventType = graph.getDesignerParent().getComponentBar().getEventType();
		//		if (eventType.equals(CreateType.ADD_ROUND_TRANSITION)) {
		//			if (e.getButton() == MouseEvent.BUTTON1) {
		//				Point p = e.getPoint();
		//				Object cell = graph.getFirstCellForLocation(p.x, p.y);
		//				if (fromPoint == null) {
		//					if (cell != null && cell instanceof OBEGraphCell) {
		//						Object userObject =
		//							((OBEGraphCell) cell).getUserObject();
		//						ActivityView activityView = (ActivityView) userObject;
		//
		//						fromPoint = e.getPoint();
		//						fromView = activityView;
		//					}
		//				} else {
		//					if (cell instanceof OBEGraphCell) {
		//						OBEGraphCell toCell = (OBEGraphCell) cell;
		//						Object userObject = toCell.getUserObject();
		//						if (userObject instanceof ActivityView) {
		//							ActivityView activityView =
		//								(ActivityView) userObject;
		//							if (!activityView.equals(fromView)) {
		//								ArrayList points = new ArrayList();
		//								OBEGraphCell fromCell =
		//									(OBEGraphCell) fromView.getCell();
		//								//points.add(fromCell.getRightPort());
		//								Point from = getCenterPoint(fromCell);
		//								Point to = getCenterPoint(toCell);
		//								points.add(new Point(20, 20));
		//								points.add(new Point(20, 50));
		//								points.add(new Point(40, 50));
		//								graph.addTransition(
		//									fromView,
		//									activityView,
		//									points);
		//								fromPoint = null;
		//								currentPoint = null;
		//								fromView = null;
		//							}
		//						}
		//					}
		//				}
		//				log.info("mousePressed");
		//			}
		//		}
	}

	public void mouseReleased(MouseEvent e)
	{
		//		eventType = graph.getDesignerParent().getComponentBar().getEventType();
		//		if (eventType.equals(CreateType.ADD_ROUND_TRANSITION)) {
		//			if (e.getButton() == MouseEvent.BUTTON3) {
		//				if (fromPoint != null) {
		//					fromPoint = null;
		//					currentPoint = null;
		//					fromView = null;
		//					graph.repaint();
		//				} else {
		//					graph.getDesignerParent().getComponentBar().setEventType(
		//						CreateType.EDIT);
		////					graph.getDesignerParent().getComponentBar().modifyBorder(
		////						CreateType.EDIT);
		//					graph.getDesignerParent().setEditMode(EditMode.EDIT);
		//				}
		//			}
		//			log.info("mouseReleased");
		//		}
	}

	public void mouseDragged(MouseEvent e)
	{}

	public void mouseMoved(MouseEvent e)
	{
		//		eventType = graph.getDesignerParent().getComponentBar().getEventType();
		//		if (eventType.equals(CreateType.ADD_ROUND_TRANSITION)) {
		//			//      log.info("mouseMoved");
		//			Point p = e.getPoint();
		//			Object cell = graph.getFirstCellForLocation(p.x, p.y);
		//			if (cell != null) {
		//				if (cell instanceof OBEGraphCell) {
		//					if (fromPoint != null) {
		//						graph.setCursor(endCenter);
		//					} else {
		//						graph.setCursor(startCenter);
		//					}
		//				}
		//			} else {
		//				graph.setCursor(normal);
		//			}
		//			if (fromPoint != null) {
		//				OBEGraphCell fromCell = (OBEGraphCell) fromView.getCell();
		//				Point from = getCenterPoint(fromCell);
		//				currentPoint = p;
		//				Graphics g = graph.getGraphics();
		//				graph.repaint();
		//				paintConnector(g);
		//			}
		//		}
	}
	private Point getCenterPoint(DefaultGraphCell cell)
	{
		Point from = null;
		Rectangle bounds = graph.getCellBounds(cell);

		double x = bounds.getCenterX();

		double y = bounds.getCenterY();
		from = new Point((int)x, (int)y);
		return from;
	}
	private void paintConnector(Graphics g)
	{
		g.setColor(Color.black);
		g.setPaintMode();

		if (fromPoint != null && currentPoint != null)
		{
			g.drawLine(
				fromPoint.x,
				fromPoint.y,
				currentPoint.x,
				currentPoint.y);
		}
	}
	public void paint(Graphics g)
	{
		if (fromPoint != null)
		{
			paintConnector(g);
		}
	}

}