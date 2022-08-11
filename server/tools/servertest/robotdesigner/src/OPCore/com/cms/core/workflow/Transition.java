package com.cms.core.workflow;

import com.cms.core.BasicElement;
import com.cms.core.workflow.condition.Condition;

public interface Transition extends BasicElement
{
	public Condition getCondition();

	public void setCondition( Condition condition);
	
	public String getFrom();

	public void setFrom( String from);

	public String getTo();

	public void setTo( String to);

	public boolean isDefault();
}
