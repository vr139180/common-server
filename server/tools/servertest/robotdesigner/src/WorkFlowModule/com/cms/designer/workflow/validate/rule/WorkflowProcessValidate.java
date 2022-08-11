package com.cms.designer.workflow.validate.rule;


import java.util.Iterator;
import java.util.List;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.coremodule.workspace.ProjectData;

/**
 * @author Administrator
 * 流程检查
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class WorkflowProcessValidate
{
	public static boolean validateProcess(ProjectData data, WorkflowPackage wp, WorkflowProcess process, List v)
	{
		boolean ret =true;
		
		String error =noStartActivity( process);
		if( error != null)
		{
			v.add( error);
			ret =false;
		}

		error =startActivityToMany( process);
		if( error != null)
		{
			v.add( error);
			ret =false;
		}
		
		return ret;
	}

	public static boolean validateProcess(ProjectData data, WorkflowPackage wp, List v)
	{
		boolean ret =true;
		Iterator ff =wp.getWorkflowProcesses().iterator();
		while( ff.hasNext())
		{
			WorkflowProcess process =( WorkflowProcess)ff.next();
			if( !validateProcess(data, wp, process, v))
				ret =false;
		}
		
		return ret;
	}
	

	/**
	 * 没有开始节点
	 * @param process
	 * @return
	 */
	public static String noStartActivity( WorkflowProcess process)
	{
		String ret =null;
		Iterator ff =process.getActivities().iterator();
		while( ff.hasNext())
		{
			Activity act =( Activity)ff.next();
			if( act.getActivityType() == Activity.START)
				return ret;
		}
		ret =process.getName()+"：必须有开始节点";
		
		return ret;
	}
	
	public static String startActivityToMany( WorkflowProcess process)
	{
		String ret =null;

		int ii =0;
		Iterator ff =process.getActivities().iterator();
		while( ff.hasNext())
		{
			Activity act =( Activity)ff.next();
			if( act.getActivityType() == Activity.START)
				++ii;
		}
		if( ii > 1)
			ret =process.getName()+"：开始节点只能有一个！";

		return ret;
	}
	
}
