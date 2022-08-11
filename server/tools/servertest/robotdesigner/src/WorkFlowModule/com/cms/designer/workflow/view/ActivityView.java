package com.cms.designer.workflow.view;

import java.io.Serializable;

import com.cms.core.workflow.activity.Activity;
import com.cms.designer.view.ElementView;
import com.cms.designer.workflow.editor.WorkflowEditor;

public class ActivityView extends ElementView implements Serializable
{
	private Activity activity;

	private WorkflowEditor editor;

	private Object cell;

	public ActivityView( Activity activity, WorkflowEditor editor)
	{
		this.activity = activity;
		this.editor = editor;
		setElement( activity);
	}

	public Activity getActivity()
	{
		return activity;
	}

	public WorkflowEditor getEditor()
	{
		return editor;
	}

	public String toString()
	{
		return activity.getId()+":"+activity.getName();
	}

	public Object getCell()
	{
		return cell;
	}

	public void setCell( Object cell)
	{
		this.cell = cell;
	}

}
