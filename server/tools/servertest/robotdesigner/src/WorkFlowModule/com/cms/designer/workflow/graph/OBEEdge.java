package com.cms.designer.workflow.graph;

import java.awt.Point;
import java.awt.Rectangle;

import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.Edge;
import org.jgraph.graph.EdgeView;
import org.jgraph.graph.PortView;

public class OBEEdge extends DefaultEdge
{

	public static boolean freeRouting = false;

	public OBEEdge(Object userObject)
	{
		super(userObject);
	}

	public static void setFreeRouting()
	{
		freeRouting = !freeRouting;
	}

	public static class SnapLeftRouting implements Edge.Routing
	{

		public void route(EdgeView edge, java.util.List points)
		{
			int n = points.size();
			Point from = edge.getPoint(0);
			if (edge.getSource() instanceof PortView)
				from = ((PortView)edge.getSource()).getLocation(null);
			else if (edge.getSource() != null)
				from = edge.getSource().getBounds().getLocation();
			Point to = edge.getPoint(n - 1);
			if (edge.getTarget() instanceof PortView)
				to = ((PortView)edge.getTarget()).getLocation(null);
			else if (edge.getTarget() != null)
				to = edge.getTarget().getBounds().getLocation();
			if (from != null && to != null)
			{
				Point[] routed;
				// Handle self references
				if (edge.getSource() == edge.getTarget()
					&& edge.getSource() != null)
				{
					Rectangle bounds =
						edge.getSource().getParentView().getBounds();
					int height = edge.getGraph().getGridSize();
					int width = (int) (bounds.getWidth() / 3);
					routed = new Point[3];
					routed[0] =
						new Point(bounds.x + width, bounds.y + bounds.height);
					routed[1] =
						new Point(
							bounds.x + width,
							bounds.y + bounds.height + height);
					routed[2] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height + height);
				}
				else
				{
					Rectangle bounds = null;
					if (edge.getTarget() != null && edge.getSource() != null)
					{
						bounds = edge.getSource().getParentView().getBounds();
					}
					int dx = Math.abs(from.x - to.x);
					int dy = Math.abs(from.y - to.y);
					if (bounds == null)
					{
						return;
					}
					else
					{
						int x2 = from.x + ((to.x - from.x) / 2);
						int y2 = from.y + ((to.y - from.y) / 2);
						if (dx < bounds.width)
						{
							routed = new Point[2];
							routed[0] = new Point(from.x, y2);
							routed[1] = new Point(to.x, y2);
						}
						else if (dy < bounds.height)
						{
							routed = new Point[2];
							routed[0] = new Point(x2, from.y);
							routed[1] = new Point(x2, to.y);
						}
						else
						{
							routed = new Point[1];
							if (from.x > to.x)
							{
								routed[0] = new Point(to.x, from.y);
							}
							else
							{
								routed[0] = new Point(from.x, to.y);
							}
						}
					}
				}
				// Set/Add Points
				for (int i = 0; i < routed.length; i++)
					if (points.size() > i + 2)
						points.set(i + 1, routed[i]);
					else
						points.add(i + 1, routed[i]);
				// Remove spare points
				while (points.size() > routed.length + 2)
				{
					points.remove(points.size() - 2);
				}
			}
		}

	}
	public static class SnapRightRouting implements Edge.Routing
	{

		public void route(EdgeView edge, java.util.List points)
		{
			int n = points.size();
			Point from = edge.getPoint(0);
			if (edge.getSource() instanceof PortView)
				from = ((PortView)edge.getSource()).getLocation(null);
			else if (edge.getSource() != null)
				from = edge.getSource().getBounds().getLocation();
			Point to = edge.getPoint(n - 1);
			if (edge.getTarget() instanceof PortView)
				to = ((PortView)edge.getTarget()).getLocation(null);
			else if (edge.getTarget() != null)
				to = edge.getTarget().getBounds().getLocation();
			if (from != null && to != null)
			{
				Point[] routed;

				// Handle self references
				if (edge.getSource() == edge.getTarget()
					&& edge.getSource() != null)
				{
					Rectangle bounds =
						edge.getSource().getParentView().getBounds();
					int height = edge.getGraph().getGridSize();
					int width = (int) (bounds.getWidth() / 3);
					routed = new Point[3];
					routed[0] =
						new Point(bounds.x + width, bounds.y + bounds.height);
					routed[1] =
						new Point(
							bounds.x + width,
							bounds.y + bounds.height + height);
					routed[2] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height + height);
				}
				else
				{
					Rectangle bounds = null;
					if (edge.getTarget() != null && edge.getSource() != null)
					{
						bounds = edge.getSource().getParentView().getBounds();
					}
					int dx = Math.abs(from.x - to.x);
					int dy = Math.abs(from.y - to.y);
					if (bounds == null)
					{
						return;
					}
					else
					{
						int x2 = from.x + ((to.x - from.x) / 2);
						int y2 = from.y + ((to.y - from.y) / 2);
						if (dx < bounds.width)
						{
							routed = new Point[2];
							routed[0] = new Point(from.x, y2);
							routed[1] = new Point(to.x, y2);
						}
						else if (dy < bounds.height)
						{
							routed = new Point[2];
							routed[0] = new Point(x2, from.y);
							routed[1] = new Point(x2, to.y);
						}
						else
						{
							routed = new Point[1];
							if (from.x > to.x)
							{
								routed[0] = new Point(from.x, to.y);
							}
							else
							{
								routed[0] = new Point(to.x, from.y);
							}
						}
					}

				}
				// Set/Add Points
				for (int i = 0; i < routed.length; i++)
					if (points.size() > i + 2)
						points.set(i + 1, routed[i]);
					else
						points.add(i + 1, routed[i]);
				// Remove spare points
				while (points.size() > routed.length + 2)
				{
					points.remove(points.size() - 2);
				}
			}
		}

	}
	//水平折线
	public static class SnapHorizontalRouting implements Edge.Routing
	{

		public void route(EdgeView edge, java.util.List points)
		{
			int n = points.size();
			Point from = edge.getPoint(0);
			if (edge.getSource() instanceof PortView)
				from = ((PortView)edge.getSource()).getLocation(null);
			else if (edge.getSource() != null)
				from = edge.getSource().getBounds().getLocation();
			Point to = edge.getPoint(n - 1);
			if (edge.getTarget() instanceof PortView)
				to = ((PortView)edge.getTarget()).getLocation(null);
			else if (edge.getTarget() != null)
				to = edge.getTarget().getBounds().getLocation();
			if (from != null && to != null)
			{
				Point[] routed;
				// Handle self references
				if (edge.getSource() == edge.getTarget()
					&& edge.getSource() != null)
				{
					Rectangle bounds =
						edge.getSource().getParentView().getBounds();
					int height = edge.getGraph().getGridSize();
					int width = (int) (bounds.getWidth() / 3);
					routed = new Point[4];
					routed[0] =
						new Point(bounds.x + width, bounds.y + bounds.height);
					routed[1] =
						new Point(
							bounds.x + width,
							bounds.y + bounds.height + height);
					routed[2] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height + height);
					routed[3] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height);
				}
				else
				{
					/*						if(freeRouting){
							Object obj0 = points.get(1);
							
							    Point orign0 = (Point)points.get(1);
							    int orignDx = Math.abs(to.x-orign0.x);
							    int dx = Math.abs(to.x-from.x);
							    
							routed = new Point[2];
							    if(dx<orignDx){
								if (from.x > to.x) {
									routed[0] = new Point(to.x, from.y);
									routed[1] = new Point(to.x, from.y+5);
								} else {
									routed[0] = new Point(from.x, to.y);
									routed[1] = new Point(from.x+5, to.y);
								}
							}
							    else if(dx>orignDx){
								Point orign1 = (Point)points.get(2);
								routed[0] = orign0;
								routed[1] = orign1;
							    }
							    
					//						  	int x2 = from.x + ((to.x - from.x) / 2);
					//						  	int y2 = from.y + ((to.y - from.y) / 2);
						}
						else{*/
					Rectangle bounds = null;
					if (edge.getTarget() != null && edge.getSource() != null)
					{
						bounds = edge.getSource().getParentView().getBounds();
					}
					int dx = Math.abs(from.x - to.x);
					routed = new Point[2];
					int x2 = from.x + ((to.x - from.x) / 2);
					int y2 = from.y + ((to.y - from.y) / 2);

					if (bounds != null && dx > bounds.width)
					{
						routed[0] = new Point(x2, from.y);
						routed[1] = new Point(x2, to.y);
					}
					else
					{
						routed[0] = new Point(from.x, y2);
						routed[1] = new Point(to.x, y2);
					}
					//						}
				}
				// Set/Add Points
				for (int i = 0; i < routed.length; i++)
					if (points.size() > i + 2)
						points.set(i + 1, routed[i]);
					else
						points.add(i + 1, routed[i]);
				// Remove spare points
				while (points.size() > routed.length + 2)
				{
					points.remove(points.size() - 2);
				}
			}
		}

	}
	//垂直折线
	public static class SnapVerticalRouting implements Edge.Routing
	{

		public void route(EdgeView edge, java.util.List points)
		{
			int n = points.size();
			Point from = edge.getPoint(0);
			if (edge.getSource() instanceof PortView)
				from = ((PortView)edge.getSource()).getLocation(null);
			else if (edge.getSource() != null)
				from = edge.getSource().getBounds().getLocation();
			Point to = edge.getPoint(n - 1);
			if (edge.getTarget() instanceof PortView)
				to = ((PortView)edge.getTarget()).getLocation(null);
			else if (edge.getTarget() != null)
				to = edge.getTarget().getBounds().getLocation();
			if (from != null && to != null)
			{
				Point[] routed;
				// Handle self references
				if (edge.getSource() == edge.getTarget()
					&& edge.getSource() != null)
				{
					Rectangle bounds =
						edge.getSource().getParentView().getBounds();
					int height = edge.getGraph().getGridSize();
					int width = (int) (bounds.getWidth() / 3);
					routed = new Point[4];
					routed[0] =
						new Point(bounds.x + width, bounds.y + bounds.height);
					routed[1] =
						new Point(
							bounds.x + width,
							bounds.y + bounds.height + height);
					routed[2] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height + height);
					routed[3] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height);
				}
				else
				{
					Rectangle bounds = null;
					if (edge.getTarget() != null && edge.getSource() != null)
					{
						bounds = edge.getSource().getParentView().getBounds();
					}
					int dy = Math.abs(from.y - to.y);
					routed = new Point[2];
					int x2 = from.x + ((to.x - from.x) / 2);
					int y2 = from.y + ((to.y - from.y) / 2);

					if (bounds != null && dy > bounds.height)
					{
						routed[0] = new Point(from.x, y2);
						routed[1] = new Point(to.x, y2);
					}
					else
					{
						routed[0] = new Point(x2, from.y);
						routed[1] = new Point(x2, to.y);
					}
				}
				// Set/Add Points
				for (int i = 0; i < routed.length; i++)
					if (points.size() > i + 2)
						points.set(i + 1, routed[i]);
					else
						points.add(i + 1, routed[i]);
				// Remove spare points
				while (points.size() > routed.length + 2)
				{
					points.remove(points.size() - 2);
				}
			}
		}

	}
	//自由线路
	public static class SnapFreeRouting implements Edge.Routing
	{

		public void route(EdgeView edge, java.util.List points)
		{
			int n = points.size();
			Point from = edge.getPoint(0);
			if (edge.getSource() instanceof PortView)
				from = ((PortView)edge.getSource()).getLocation(null);
			else if (edge.getSource() != null)
				from = edge.getSource().getBounds().getLocation();
			Point to = edge.getPoint(n - 1);
			if (edge.getTarget() instanceof PortView)
				to = ((PortView)edge.getTarget()).getLocation(null);
			else if (edge.getTarget() != null)
				to = edge.getTarget().getBounds().getLocation();
			if (from != null && to != null)
			{
				Point[] routed;
				// Handle self references
				if (edge.getSource() == edge.getTarget()
					&& edge.getSource() != null)
				{
					Rectangle bounds =
						edge.getSource().getParentView().getBounds();
					int height = edge.getGraph().getGridSize();
					int width = (int) (bounds.getWidth() / 3);
					routed = new Point[4];
					routed[0] =
						new Point(bounds.x + width, bounds.y + bounds.height);
					routed[1] =
						new Point(
							bounds.x + width,
							bounds.y + bounds.height + height);
					routed[2] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height + height);
					routed[3] =
						new Point(
							bounds.x + 2 * width,
							bounds.y + bounds.height);
				}
				else
				{
					Rectangle bounds = null;
					if (edge.getTarget() != null && edge.getSource() != null)
					{
						bounds = edge.getSource().getParentView().getBounds();
					}
					int dx = Math.abs(to.x - from.x);
					int dy = Math.abs(from.y - to.y);
					routed = new Point[2];
					Point orign0;
					Point orign1;

					if (points.size() == 4)
					{
						routed = new Point[2];
						orign0 = (Point)points.get(1);
						orign1 = (Point)points.get(2);

						int orignDx = Math.abs(to.x - orign0.x);
						if (orign0.x == orign1.x)
						{
							//横向移动

							if (dx < (orignDx + bounds.width))
							{
								//改变坐标
								routed = new Point[1];
								routed[0] = new Point(from.x, to.y);
								//								if (from.x > to.x) {
								//									routed[0] = new Point(to.x, from.y);
								//									routed[1] = new Point(to.x, from.y+5);
								//								} else {
								//									routed[0] = new Point(from.x, to.y);
								//									routed[1] = new Point(from.x+5, to.y);
								//								}
							}
							else if (dx > (orignDx + bounds.width))
							{
								//不改变坐标
								routed[0] = orign0;
								routed[1] = orign1;
							}
						}
						else if (orign0.y == orign1.y)
						{
							//纵向移动

						}
					}
					else if (points.size() == 3)
					{
						routed = new Point[1];
						orign0 = (Point)points.get(1);
						routed[0] = new Point(from.x, to.y);
					}

					/*					int x2 = from.x + ((to.x - from.x) / 2);
										int y2 = from.y + ((to.y - from.y) / 2);
											
										if(bounds!=null&&dy>bounds.height){
											routed[0] = new Point(from.x, y2);
											routed[1] = new Point(to.x, y2);
										}
										else{
											routed[0] = new Point(x2, from.y);
											routed[1] = new Point(x2, to.y);
										}
					*/
				}
				// Set/Add Points
				for (int i = 0; i < routed.length; i++)
					if (points.size() > i + 2)
						points.set(i + 1, routed[i]);
					else
						points.add(i + 1, routed[i]);
				// Remove spare points
				while (points.size() > routed.length + 2)
				{
					points.remove(points.size() - 2);
				}
			}
		}

	}

}
