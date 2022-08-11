package com.cms.core.workflow.condition;

public abstract class AbstractCondition implements Condition
{
	private boolean isdefault_ =false;
	private String condition ="";

	/** Construct a new AbstractCondition. */
	public AbstractCondition()
	{
	}

	public String getText()
	{
		return condition;
	}
	
	public void setText( String t)
	{
		condition =t;
		condition =condition.trim();
	}

	public boolean isIsdefault() {
		return isdefault_;
	}

	public void setIsdefault(boolean isdefault_) {
		this.isdefault_ = isdefault_;
		
		if( isdefault_)
			condition ="";
	}
}
