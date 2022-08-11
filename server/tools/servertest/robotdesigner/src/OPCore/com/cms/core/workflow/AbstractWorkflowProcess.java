package com.cms.core.workflow;

import java.util.List;
import java.util.ArrayList;

import com.cms.core.*;

public abstract class AbstractWorkflowProcess extends AbstractElement implements WorkflowProcess
{
	private List applications;

	private List activities;

	private List transitions;

	protected transient int _transition_id = 0;

	protected transient int _activity_id = 0;
	
	public AbstractWorkflowProcess( String id, String name)
	{
		super( id, name);

		this.applications = new ArrayList();
		this.activities = new ArrayList();
		this.transitions = new ArrayList();
	}

	public synchronized String getNextTransitionId()
	{
		String ret = "" + (++_transition_id);
		getExtendedAttributes().put( "auto.transition.autoid", new Integer( _transition_id));
		return ret;
	}

	public synchronized String getNextActivityId()
	{
		_activity_id++;
		String ret = ""+_activity_id;
		getExtendedAttributes().put( "auto.activity.autoid", new Integer( _activity_id));
		return ret;
	}
	
	public synchronized void initTransitionId( String id)
	{
		if( id == null || id.equals( ""))
			return;
		_transition_id = Integer.parseInt( id);
	}

	public synchronized void initActivityId( String id)
	{
		if( id == null || id.equals( ""))
			return;
		_activity_id = Integer.parseInt( id);
	}
	
	public List getApplications()
	{
		return applications;
	}
	

	public List getActivities()
	{
		return activities;
	}

	public List getTransitions()
	{
		return transitions;
	}

}
