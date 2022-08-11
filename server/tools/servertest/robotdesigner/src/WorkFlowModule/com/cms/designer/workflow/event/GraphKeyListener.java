package com.cms.designer.workflow.event;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import com.cms.designer.workflow.OBEGraphWorkflow;

public class GraphKeyListener extends KeyAdapter
{
	private OBEGraphWorkflow graph;
	
	public GraphKeyListener( OBEGraphWorkflow graph)
	{
		this.graph = graph;
	}
	
	public void keyPressed(KeyEvent evt)
	{
		switch (evt.getKeyCode())
		{
			case KeyEvent.VK_DELETE :
				graph.delete();
				break;
			case KeyEvent.VK_ENTER :
				graph.edit();
				evt.consume();
				break;
			case KeyEvent.VK_ESCAPE :
				//connectMouseListener.reset();
				break;
		}
	}
}