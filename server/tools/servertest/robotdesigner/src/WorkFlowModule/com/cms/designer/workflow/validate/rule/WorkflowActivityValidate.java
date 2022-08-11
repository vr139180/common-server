package com.cms.designer.workflow.validate.rule;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.core.workflow.activity.Tool;
import com.cms.core.workflow.activity.ToolSet;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.workflow.WorkflowModuleData;

/**
 * @author Administrator
 * 活动检查
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class WorkflowActivityValidate
{
	/**
	 * 验证所有节点
	 * @param process
	 * @param v
	 * @return
	 */
	public static boolean validateActivity( ProjectData projectdata,WorkflowModuleData data, WorkflowProcess process, List v)
	{
		
		boolean ret =true;
		String error =null;
		
		error =noSplitOfStart( process);
		if( error != null)
		{
			v.add( error);
			ret =false;
		}

		Iterator ff =process.getActivities().iterator();
		while( ff.hasNext())
		{
			Activity act =( Activity)ff.next();
			error =noSplitOrJoinOfOther( process, act);
			if( error != null)
			{
				v.add( error);
				ret =false;
			}
			
			error =noActions( process, act);
			if( error != null)
			{
				v.add( error);
				ret =false;
			}
		}

		//验证
		return ret;
	}
	
	/**
	 * 开始节点没有出口
	 * @param process
	 * @param act
	 * @return
	 */
	public static String noSplitOfStart( WorkflowProcess process)
	{
		String ret =null;
		
		Iterator ff =process.getActivities().iterator();
		Activity act =null;
		while( ff.hasNext())
		{
			act =( Activity)ff.next();
			if( act.getActivityType() == Activity.START)
				break;
			act =null;
		}
		
		if( act != null)
		{
			Iterator ii =process.getTransitions().iterator();
			List vv =new ArrayList();
			//保证含有一个缺省转移或者，没有条件为空的转移
			while( ii.hasNext())
			{
				Transition tt =( Transition)ii.next();
				if( tt.isDefault())
					vv.add( tt.getFrom());
			}
			if( !vv.contains( act.getId()))
				ret =act.getId()+"->"+act.getName()+"：警告(开始节点)至少必须设置一个缺省的转出！";
		}
		
		return ret;
	}
	
	/**
	 * 除了开始和结束节点，其他的节点必须有出口和入口
	 * @param process
	 * @param act
	 * @return
	 */
	public static String noSplitOrJoinOfOther( WorkflowProcess process, Activity act)
	{
		String ret =null;
		
		if( act.getActivityType() == Activity.START)
			return ret;

		Iterator ii =process.getTransitionsFrom( act.getId()).iterator();

		int bdef =0;
		while( ii.hasNext())
		{
			Transition tt =( Transition)ii.next();
			if( tt.isDefault())
				++bdef;
		}

		if( bdef > 1)
			ret =act.getId()+"->"+act.getName()+"：节点最多设置一个缺省条件！";
		else if( bdef == 0)
			ret =act.getId()+"->"+act.getName()+"：节点必须设置一个缺省条件！";

		return ret;
	}
	
	public static String noActions( WorkflowProcess process, Activity act)
	{
		String ret =null;

		if( act.getTset_().getTools().size() == 0)
			ret =act.getId()+"->"+act.getName()+"节点，必须设置至少一个action";

		return ret;
	}

}
