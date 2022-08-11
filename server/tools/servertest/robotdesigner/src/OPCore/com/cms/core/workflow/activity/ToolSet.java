package com.cms.core.workflow.activity;

import java.util.HashMap;
import java.util.Map;

import com.cms.core.workflow.*;

public class ToolSet implements Implementation
{
	private Map tools =new HashMap();

	/** Construct a new ToolSet. */
	public ToolSet()
	{
	}

	/**
	 * Get a List of all tools in the set.
	 * 
	 * @return a List of tools
	 */
	public void addTools( Tool tool)
	{
		tools.put( tool.getName(), tool);
	}

	public Tool getTool( String classId)
	{
		return (Tool)tools.get( classId);
	}
	
	public Map getTools()
	{
		return tools;
	}

	public Tool removeTool( String classId)
	{
		return ( Tool)tools.remove( classId);
	}
	
	public void removeAll()
	{
		tools.clear();
	}
}
