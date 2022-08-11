package com.cms.core.workflow.condition;

/**
 * Interface which represents a condition.
 * 
 * @author jeffery
 */
public interface Condition
{
	public boolean isIsdefault();

	public void setIsdefault(boolean isdefault_);
	
	public String getText();
	public void setText( String t);
}
