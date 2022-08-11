package com.cms.designer.workflow.view;

import java.io.Serializable;

import com.cms.core.workflow.Transition;
import com.cms.designer.view.ElementView;

public class TransitionView extends ElementView implements Serializable
{

	private Transition transition;

	private Object cell;

	public TransitionView( Transition transition)
	{
		this.transition = transition;
		setElement( transition);
	}

	public Transition getTransition()
	{
		return transition;
	}

	public Object getCell()
	{
		return cell;
	}

	public void setCell( Object cell)
	{
		this.cell = cell;
	}

	public String toString()
	{
		return transition.getName();
	}
}
