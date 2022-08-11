package com.cms.designer.workflow.graph;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Port;

public class OBEGraphCell extends DefaultGraphCell
{
	private DefaultPort leftPort;
	private DefaultPort rightPort;

	public OBEGraphCell(Object userObject)
	{
		super(userObject);
		leftPort = new DefaultPort("left");
		rightPort = new DefaultPort("right");

		add(leftPort);
		add(rightPort);
	}
	
	public Port getLeftPort()
	{
		return leftPort;
	}

	public Port getRightPort()
	{
		return rightPort;
	}

}
