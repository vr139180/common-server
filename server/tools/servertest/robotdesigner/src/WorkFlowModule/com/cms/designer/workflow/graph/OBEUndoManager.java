package com.cms.designer.workflow.graph;

import javax.swing.event.UndoableEditEvent;

import org.jgraph.graph.GraphUndoManager;

import com.cms.designer.workflow.OBEGraphWorkflow;

public class OBEUndoManager extends GraphUndoManager
{
	private OBEGraphWorkflow graph;
	
	public OBEUndoManager( OBEGraphWorkflow graph)
	{
		this.graph = graph;
		this.discardAllEdits();
	}
	public void undoableEditHappened(UndoableEditEvent e)
	{
		// First Invoke Superclass
		super.undoableEditHappened(e);
		// Then Update Undo/Redo Buttons
		graph.updateHistoryButtons();
	}

}