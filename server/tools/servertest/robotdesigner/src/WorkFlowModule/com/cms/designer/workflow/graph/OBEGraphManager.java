package com.cms.designer.workflow.graph;

import java.awt.Graphics;

import com.cms.designer.workflow.OBEGraphWorkflow;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEGraphManager.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class OBEGraphManager
{
	private OBEGraphWorkflow graph;
	//回退管理者
	private OBEUndoManager undoManager;
	//
	private OBEMarqueeHandler marqueeHandler;

	public OBEGraphManager( OBEGraphWorkflow graph)
	{
		this.graph = graph;
		this.undoManager = new OBEUndoManager(graph);

		this.marqueeHandler = new OBEMarqueeHandler();
	}
	public OBEUndoManager getUndoManager()
	{
		return undoManager;
	}
	public OBEMarqueeHandler getMarqueeHandler()
	{
		return marqueeHandler;
	}
	public void paint(Graphics g)
	{
		/*
		if (createLineTransitionListener != null)
		{
			createLineTransitionListener.paint(g);
		}
		if (createRoundTransitionListener != null)
		{
			createRoundTransitionListener.paint(g);
		}
		*/
	}
}