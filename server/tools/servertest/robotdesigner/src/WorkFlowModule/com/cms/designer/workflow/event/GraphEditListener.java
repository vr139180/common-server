package com.cms.designer.workflow.event;

import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.EdgeView;

import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.util.EditMode;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.graph.OBEEdge;

public class GraphEditListener
	extends MouseAdapter
	implements MouseMotionListener
{
	private OBEGraphWorkflow graph;
	private OBEModuleStandard module =null;
	
	private Point currentPoint;
	private int x = 0;
	private int y = 0;
	private int index = 0;
	private Point point;

	public GraphEditListener( OBEGraphWorkflow graph, OBEModuleStandard module)
	{
		this.graph = graph;
		this.module =module;
	}
	
	public void mouseMoved(MouseEvent evt)
	{
		//		graph.setCursor(OBEDesigner.cursor);
		graph.getDesignerParent().setStatus(
			"x,y:[" + evt.getX() + "," + evt.getY() + "]");
	}
	
	public void mousePressed(MouseEvent evt)
	{
		x = evt.getX();
		y = evt.getY();
		currentPoint = evt.getPoint();
		if (SwingUtilities.isLeftMouseButton(evt))
		{
			if (graph.getSelectionCount() == 1)
			{
				DefaultGraphCell cell =
					(DefaultGraphCell)graph.getSelectionCell();
				Map map = cell.getAttributes();
				if (cell instanceof OBEEdge)
				{
					OBEEdge edgeCell = (OBEEdge)cell;
					EdgeView edge =
						(EdgeView)graph.getGraphLayoutCache().getMapping(
							edgeCell,
							false);
				}
				if (currentPoint != null)
					evt.consume();
			}
		}
	}
	
	public void mouseDragged(MouseEvent evt)
	{}

	public void mouseReleased(MouseEvent evt)
	{
		if (evt.getClickCount() == 2)
		{
			graph.edit();
		}
		else if( evt.isPopupTrigger() && graph.getDesignerParent().getEditMode().equals(EditMode.EDIT))
		{
			graph.showPopup(evt.getPoint());
		}
	}
}