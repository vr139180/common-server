package com.cms.designer.util;

import java.awt.Point;
import java.awt.Rectangle;
import java.util.List;

import org.jgraph.JGraph;
import org.jgraph.graph.CellView;
import org.jgraph.graph.EdgeView;
import org.jgraph.graph.PortView;

import com.cms.designer.workflow.OBEGraphWorkflow;

/**
 * @author Administrator
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class CalculatePointUtil
{

	public static void optEdgeView( CellView view, Point mousePoint)
	{
		if( view instanceof EdgeView)
		{
			EdgeView edgeView = (EdgeView) view;
			OBEGraphWorkflow graph = (OBEGraphWorkflow) edgeView.getGraph();
			if( graph.isAutoOPTEdge())
			{
				List points = edgeView.getPoints();

				CalculatePointUtil cpu = new CalculatePointUtil( edgeView);

				if( points.size() == 2)
				{
					cpu.opt2( points, mousePoint);
				}
				else if( points.size() == 3)
				{
					cpu.opt3( points);
				}
				else if( points.size() == 4)
				{
					cpu.opt4( points, mousePoint);
				}
			}
		}
	}

	private CalculatePointUtil( EdgeView edgeView)
	{

		try
		{
			graph = edgeView.getGraph();

			List points = edgeView.getPoints();
			PortView portViewS = (PortView) points.get( 0);
			PortView portViewT = (PortView) points.get( points.size() - 1);

			sBounds = portViewS.getParentView().getBounds();
			tBounds = portViewT.getParentView().getBounds();
			w = (int) sBounds.getWidth();
			h = (int) sBounds.getHeight();

			s = findCenterPoint( sBounds);
			t = findCenterPoint( tBounds);

		}
		catch( Exception e)
		{
		}
	}

	private Point findCenterPoint( Rectangle bounds)
	{
		Point point = null;

		double x = bounds.getCenterX();
		double y = bounds.getCenterY();
		point = new Point( (int) x, (int) y);

		return point;
	}

	public void opt2( List points, Point mousePoint)
	{
		Point m = null;
		Point p_b = null;
		try
		{
			graph.snap( mousePoint);
			if( s.x == t.x)
			{
				if( mousePoint.x != s.x)
				{
					if( Math.abs( mousePoint.x - s.x) < w)
					{
						if( mousePoint.x < s.x && mousePoint.x > w)
						{
							m = new Point( s.x - w, s.y);
							p_b = new Point( s.x - w, t.y);
						}
						else
						{
							m = new Point( s.x + w, s.y);
							p_b = new Point( s.x + w, t.y);
						}
					}
					else
					{
						m = new Point( mousePoint.x, s.y);
						p_b = new Point( mousePoint.x, t.y);
					}
				}
			}
			else if( s.y == t.y)
			{
				if( mousePoint.y != s.y)
				{
					if( Math.abs( mousePoint.y - s.y) < h)
					{
						if( mousePoint.x < s.y && mousePoint.x > h)
						{
							m = new Point( s.x, s.y - h);
							p_b = new Point( t.x, t.y - h);
						}
						else
						{
							m = new Point( s.x, mousePoint.y);
							p_b = new Point( t.x, mousePoint.y);
						}
					}
					else
					{
						m = new Point( s.x, mousePoint.y);
						p_b = new Point( t.x, mousePoint.y);
					}
				}
			}
			else
			{
				//不在一条水平线上 并且不在竖直线上
				Point p_a = null;
				Point p_t = null;
				int dx = Math.abs( s.x - t.x);
				int dy = Math.abs( s.y - t.y);
				if( dx > w && dy > h)
				{
					//加一点
					if( dx > dy)
					{
						p_a = new Point( t.x, s.y);
					}
					else
					{
						p_a = new Point( s.x, t.y);
					}
					points.add( 1, p_a);
				}
				else
				{
					//距离不够，加两点
					if( dx > dy)
					{
						int minY = Math.min( s.y, t.y);
						int maxY = Math.max( s.y, t.y);
						if( minY - h > 0)
						{
							p_a = new Point( s.x, minY - h);
							p_t = new Point( t.x, minY - h);
						}
						else
						{
							p_a = new Point( s.x, maxY + h);
							p_t = new Point( t.x, maxY + h);
						}
					}
					else
					{
						int minX = Math.min( s.x, t.x);
						int maxX = Math.max( s.x, t.x);
						if( minX - w > 0)
						{
							p_a = new Point( minX - w, s.y);
							p_t = new Point( minX - w, t.y);
						}
						else
						{
							p_a = new Point( maxX + w, s.y);
							p_t = new Point( maxX + w, t.y);
						}
					}
					points.add( 1, p_a);
					points.add( 2, p_t);
				}
			}
			if( m != null && p_b != null)
			{
				points.add( 1, m);
				points.add( 2, p_b);
			}
		}
		catch( Exception e)
		{
		}

	}

	private void opt3( List points)
	{

		try
		{
			Point m = (Point) points.get( 1);
			Point p_b = null;
			checkScope( m);

			/*
			 * | | | | | | | | w | | b | |w
			 * ---------------------------------------------------------- | A | |A |
			 * ----|-- |-------- --|---------- | | | | b | | n | |b | | | | | | | |
			 * ----|---|-------|---|---------------
			 * -----------------|-------|----------------------------------- w | | | |w | |
			 * b | | | | | |
			 */

			if( s.x == t.x || s.y == t.y)
			{
				//在X或者Y上
				if( (m.x == s.x && s.x == t.x) || (m.y == s.y && s.y == t.y))
				{
					points.remove( m);
				}
				else
				{
					//不是三点一线
					if( s.x == t.x)
					{
						//但是如果m与st线的距离大于w/2
						if( Math.abs( m.x - s.x) > w / 2 + 2)
						{
							m.y = s.y;
							p_b = new Point( m.x, t.y);
						}
						else
						{
							points.remove( m);
						}

					}
					else
					{
						//但是如果m与st线的距离大于w/2
						if( Math.abs( m.y - s.y) > h / 2 + 2)
						{
							m.x = s.x;
							p_b = new Point( t.x, m.y);
						}
						else
						{
							points.remove( m);
						}

					}
				}
			}
			else
			{
				//不在一条水平线上 并且不在竖直线上
				int dx = Math.abs( s.x - t.x);
				int dy = Math.abs( s.y - t.y);
				if( dx > w || dy > h)
				{// for test
					//距离足够
					int x, y, x_width, y_width;
					x = Math.min( sBounds.x, tBounds.x);
					y = Math.min( sBounds.y, tBounds.y);
					x_width = Math.abs( sBounds.x - tBounds.x) + w;
					y_width = Math.abs( sBounds.y - tBounds.y) + h;

					Rectangle bigScope = new Rectangle( x, y, x_width, y_width);

					if( bigScope.contains( m))
					{
						int sx, sy, sx_width, sy_width;
						sx = x + w;
						sy = y + h;
						sx_width = x_width - 2 * w;
						sy_width = y_width - 2 * h;

						Rectangle smallScope = new Rectangle( sx, sy, sx_width, sy_width);
						if( smallScope.contains( m))
						{
							m.x = s.x;

							p_b = new Point( s.x, m.y);
						}
						else
						{
							//在四个角里
							if( sBounds.contains( m))
							{
								m.x = s.x;
								m.y = t.y;
							}
							else if( tBounds.contains( m))
							{
								m.x = t.x;
								m.y = s.y;
							}
							else if( (m.x >= t.x - w / 2 && m.x <= t.x + w / 2)
									&& (m.y >= s.y - h / 2 && m.y <= s.y + h / 2))
							{
								m.x = t.x;
								m.y = s.y;
							}
							else if( (m.x >= s.x - w / 2 && m.x <= s.x + w / 2)
									&& (m.y >= t.y - h / 2 && m.y <= t.y + h / 2))
							{
								m.x = s.x;
								m.y = t.y;
							}
							else
							{
								//在四条边上(可合并为两种情况)
								if( m.x >= s.x - w / 2 && m.x <= s.x + w / 2)
								{
									m.x = s.x;

									p_b = new Point( t.x, m.y);
								}
								else if( m.x >= t.x - w / 2 && m.x <= t.x + w / 2)
								{
									m.x = s.x;

									p_b = new Point( t.x, m.y);
								}
								else if( m.y >= s.y - h / 2 && m.y <= s.y + h / 2)
								{
									m.y = s.y;

									p_b = new Point( m.x, t.y);
								}
								else if( m.y >= t.y - h / 2 && m.y <= t.y + h / 2)
								{
									m.y = s.y;

									p_b = new Point( m.x, t.y);
								}
							}
						}
					}
					else
					{
						if( (m.x < Math.min( s.x, t.x) - w / 2 || m.x > Math.max( s.x, t.x) + w / 2)
								&& (m.y < Math.min( s.y, t.y) - h / 2 || m.y > Math.max( s.y, t.y)
										+ h / 2))
						{
							//在w区间
							m.y = s.y;

							p_b = new Point( m.x, t.y);
						}
						else
						{
							//在B区间
							if( m.x < Math.min( s.x - w / 2, t.x - w / 2)
									|| m.x > Math.max( s.x + w / 2, t.x + w / 2))
							{
								m.y = s.y;

								p_b = new Point( m.x, t.y);
							}
							else
							{
								m.x = s.x;

								p_b = new Point( t.x, m.y);
							}
						}
					}
				}
				else if( dx > w || dy > h)
				{
					//距离不够
					int minX = Math.min( s.x, t.x) - w;
					int minY = Math.min( s.y, t.y) - h;
					if( dy > h)
					{
						if( (m.x > minX + w / 2) && (m.x < Math.max( s.x, t.x) + w / 2))
						{
							m.x = Math.min( s.x, t.x);
							if( !(m.y > Math.min( s.y, t.y) && m.y < Math.max( s.y, t.y)))
							{
								m.y = Math.abs( s.y + t.y) / 2;
							}
							p_b = new Point( Math.max( s.x, t.x), m.y);
						}
						else
						{

						}
					}
				}
				else
				{
					//距离不够
					int minX = Math.min( s.x, t.x) - w * 3 / 4;
					int minY = Math.min( s.y, t.y) - h * 3 / 4;

					if( dx <= dy)
					{
						if( minX > 0)
						{
							m.x = minX;
							m.y = s.y;

							p_b = new Point( minX, t.y);
						}
						else
						{
							int maxX = Math.max( s.x, t.x) + w;
							m.x = maxX;
							m.y = s.y;

							p_b = new Point( maxX, t.y);
						}
					}
					else
					{
						if( minY > 0)
						{
							m.x = s.x;
							m.y = minY;

							p_b = new Point( t.x, minY);
						}
						else
						{
							int maxY = Math.max( s.y, t.y) + h;
							m.x = s.x;
							m.y = maxY;

							p_b = new Point( t.x, maxY);
						}
					}
				}//距离不够
			} //不在一条水平线上 并且不在竖直线上

			if( p_b != null)
			{
				points.add( 2, p_b);
			}
		}
		catch( Exception e)
		{
		}
	}

	private void opt4( List points, Point mp)
	{

		try
		{
			Point m1 = (Point) points.get( 1);
			Point m2 = (Point) points.get( 2);

			checkScope( m1);
			checkScope( m2);

			Rectangle bigScope = null;
			int x, y, x_width, y_width;

			x = Math.min( sBounds.x, tBounds.x);
			y = Math.min( sBounds.y, tBounds.y);
			x_width = Math.abs( sBounds.x - tBounds.x) + w;
			y_width = Math.abs( sBounds.y - tBounds.y) + h;

			bigScope = new Rectangle( x, y, x_width, y_width);

			//判断
			if( (s.x == t.x || s.y == t.y) && (sBounds.contains( mp) || tBounds.contains( mp)))
			{
				points.remove( m1);
				points.remove( m2);
			}
			else if( sBounds.contains( m2) || tBounds.contains( m2))
			{
				points.remove( m2);
				opt3( points);
			}
			else if( bigScope.contains( m1))
			{
				points.remove( m1);
				opt3( points);
			}
			else if( bigScope.contains( m2))
			{
				points.remove( m2);
				opt3( points);
			}
			else
			{
				points.remove( m1);
				opt3( points);
			}

		}
		catch( Exception e)
		{
		}
	}

	public Point checkScope( Point p)
	{
		if( p.x < 0)
			p.x = graph.getGridSize() / 2;
		if( p.y < 0)
			p.y = graph.getGridSize() / 2;
		return p;
	}

	public Rectangle checkScope( Rectangle rc)
	{
		Point p = rc.getLocation();
		if( p.x < 0)
			p.x = graph.getGridSize() / 2;
		if( p.y < 0)
			p.y = graph.getGridSize() / 2;
		rc.setLocation( p);
		return rc;
	}

	private int w, h;

	private Point s, t;

	private Rectangle sBounds, tBounds;

	private JGraph graph;
}
