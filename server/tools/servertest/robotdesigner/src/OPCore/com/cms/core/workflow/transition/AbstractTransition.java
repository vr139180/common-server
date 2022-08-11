package com.cms.core.workflow.transition;

import com.cms.core.*;
import com.cms.core.workflow.*;
import com.cms.core.workflow.condition.BasicCondition;
import com.cms.core.workflow.condition.Condition;

public abstract class AbstractTransition extends AbstractElement implements Transition
{
	private Condition condition;

	private String from;

	private String to;

	public AbstractTransition( String id, String name, String from, String to)
	{
		super( id, name);

		condition =new BasicCondition();
		
		setFrom( from);
		setTo( to);
	}
	
	public boolean isDefault()
	{
		return condition.isIsdefault();
	}

	public Condition getCondition()
	{
		return condition;
	}

	public void setCondition( Condition condition)
	{
		this.condition = condition;
	}

	public String getFrom()
	{
		return from;
	}

	public void setFrom( String from)
	{
		if( from == null){ throw new IllegalArgumentException( "Loop 'from' attribute required"); }
		this.from = from;
	}

	public String getTo()
	{
		return to;
	}

	public void setTo( String to)
	{
		if( to == null){ throw new IllegalArgumentException( "Loop 'to' attribute required"); }
		this.to = to;
	}

}
