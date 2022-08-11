package com.cms.designer.util;

public class GraphIndex
{
	public double x = 0;
	public double y = 0;

	public GraphIndex()
	{}

	public GraphIndex(double x, double y)
	{
		this.x = x;
		this.y = y;
	}

	public GraphIndex copy()
	{
		return new GraphIndex(x, y);
	}

	public GraphIndex copy(double xz, double yz)
	{
		return new GraphIndex(x + xz, y + yz);
	}
}
