package com.cms.designer.workflow.graph;

import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.Port;

public class OBEGraphModel extends DefaultGraphModel
{
	private boolean modified = false;

	public OBEGraphModel()
	{
	}

	public void clear()
	{
		remove(getRoots(this));
	}

	public boolean isModified()
	{
		return modified;
	}

	public boolean isPort(Object obj)
	{
		return obj instanceof Port;
	}

	public void setModified(boolean modified)
	{
		this.modified = modified;
	}
}
