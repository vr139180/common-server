package com.cms.designer.workflow.graph;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import org.jgraph.JGraph;
import org.jgraph.graph.BasicMarqueeHandler;
import org.jgraph.graph.CellView;
import org.jgraph.graph.EdgeView;

/**
 * @author Administrator
 *
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */
public class OBEMarqueeHandler extends BasicMarqueeHandler
{
	/* Restore previous cursor after operation. */
	protected transient Cursor previousCursor = null;

	/* The rectangle that defines the current marquee selection. */
	protected Rectangle marqueeBounds, lastVisibleRect;

	/* The start start and current point of the marquee session. */
	protected Point startPoint, currentPoint;

	/* Return true if this handler should be preferred over other handlers. */
	public boolean isForceMarqueeEvent(MouseEvent event)
	{
		return event.isAltDown();
	}

	/**
	  * Stops the current marquee selection.
	  */
	public void mouseReleased(MouseEvent e)
	{
		try
		{
			if (e != null && !e.isConsumed() && marqueeBounds != null)
			{
				if (!(e.getSource() instanceof JGraph))
					throw new IllegalArgumentException(
						"MarqueeHandler cannot "
							+ "handle event from unknown source: "
							+ e);
				JGraph graph = (JGraph)e.getSource();
				Rectangle bounds =
					graph.fromScreen(new Rectangle(marqueeBounds));
				CellView[] views = graph.getGraphLayoutCache().getRoots(bounds);

				ArrayList list = new ArrayList();
				for (int i = 0; i < views.length; i++)
					if (graph
						.getGraphLayoutCache()
						.isVisible(views[i].getAttributes())
						&& bounds.contains(views[i].getBounds()))
						if (!(views[i] instanceof EdgeView))
						{
							list.add(views[i].getCell());
						}
				Object[] cells = list.toArray();
				graph.getUI().selectCellsForEvent(graph, cells, e);
				graph.setCursor(previousCursor);
				Rectangle dirty = new Rectangle(marqueeBounds);
				dirty.width++;
				dirty.height++;
				graph.repaint(dirty);

				e.consume();
			}
		}
		finally
		{
			currentPoint = null;
			startPoint = null;
			marqueeBounds = null;
			previousCursor = null;
		}
	}

	/**
	  * Includes the specified startPoint in the marquee selection. Calls
	  * overlay.
	  */
	public void mouseDragged(MouseEvent e)
	{
		if (!e.isConsumed() && startPoint != null)
		{
			if (!(e.getSource() instanceof JGraph))
				throw new IllegalArgumentException(
					"MarqueeHandler cannot handle event from unknown source: "
						+ e);
			JGraph graph = (JGraph)e.getSource();
			Object cell =
				graph.getFirstCellForLocation(startPoint.x, startPoint.y);
			if (cell == null)
			{
				Graphics g = graph.getGraphics();
				Color bg = graph.getBackground();
				Color fg = graph.getMarqueeColor();
				g.setColor(fg);
				g.setXORMode(bg);
				overlay(g);
				currentPoint = e.getPoint();
				marqueeBounds = new Rectangle(startPoint);
				marqueeBounds.add(currentPoint);
				g.setColor(bg);
				g.setXORMode(fg);
				overlay(g);
			}
			e.consume();
		}
	}

	/**
		* Called after the component was repainted (ie. after autoscroll).
	  * This is used to indicate that the graphics is no more dirty.
	  */
	public void paint(Graphics g)
	{
		overlay(g);
	}

	protected void overlay(Graphics g)
	{
		if (marqueeBounds != null)
			g.drawRect(
				marqueeBounds.x,
				marqueeBounds.y,
				marqueeBounds.width,
				marqueeBounds.height);
	}

	/**
	  * Start the marquee at the specified startPoint. This invokes
	  * expandMarqueeToPoint to initialize marquee selection.
	  */
	public void mousePressed(MouseEvent e)
	{
		if (!e.isConsumed())
		{
			if (!(e.getSource() instanceof JGraph))
				throw new IllegalArgumentException(
					"MarqueeHandler cannot handle event from unknown source: "
						+ e);
			JGraph graph = (JGraph)e.getSource();
			startPoint = e.getPoint();
			marqueeBounds = new Rectangle(startPoint);
			previousCursor = graph.getCursor();
			graph.setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
			lastVisibleRect = graph.getVisibleRect();
			//			graph.getSelectionCell()
			e.consume();
		}

	}

	/**
	 * Empty.
	 */
	public void mouseMoved(MouseEvent e)
	{}
}
