package com.cms.designer.workflow.graph;

import com.cms.core.workflow.activity.Activity;
import com.cms.designer.util.GraphIndex;

public class ActivityColumnEntry
{
	private Activity activity;
	private GraphIndex index;

	public ActivityColumnEntry(Activity activity, GraphIndex index)
	{
		this.activity = activity;
		this.index = index;
	}

	public String toString()
	{
		return super.toString();
	}

	public Activity getActivity()
	{
		return activity;
	}

	public GraphIndex getIndex()
	{
		return index;
	}
}
