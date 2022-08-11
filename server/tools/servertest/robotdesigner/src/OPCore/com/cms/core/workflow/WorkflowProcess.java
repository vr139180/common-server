package com.cms.core.workflow;

import java.util.List;

import com.cms.core.BasicElement;

public interface WorkflowProcess extends BasicElement
{
	public List getActivities();

	public List getTransitions();
	
	public List getTransitionsFrom( String id);

	public String getNextTransitionId();
	
	public String getNextActivityId();
	
	public void initTransitionId( String id);
	
	public void initActivityId( String id);
	
}
