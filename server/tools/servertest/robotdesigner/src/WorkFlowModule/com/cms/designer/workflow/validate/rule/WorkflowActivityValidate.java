package com.cms.designer.workflow.validate.rule;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.workflow.WorkflowModuleData;

/**
 * @author Administrator
 * ����
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class WorkflowActivityValidate
{
	/**
	 * ��֤���нڵ�
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

		//��֤
		return ret;
	}
	
	/**
	 * ��ʼ�ڵ�û�г���
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
			//��֤����һ��ȱʡת�ƻ��ߣ�û������Ϊ�յ�ת��
			while( ii.hasNext())
			{
				Transition tt =( Transition)ii.next();
				if( tt.isDefault())
					vv.add( tt.getFrom());
			}
			if( !vv.contains( act.getId()))
				ret =act.getId()+"->"+act.getName()+"������(��ʼ�ڵ�)���ٱ�������һ��ȱʡ��ת����";
		}
		
		return ret;
	}
	
	/**
	 * ���˿�ʼ�ͽ����ڵ㣬�����Ľڵ�����г��ں����
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
			ret =act.getId()+"->"+act.getName()+"���ڵ��������һ��ȱʡ������";
		else if( bdef == 0)
			ret =act.getId()+"->"+act.getName()+"���ڵ��������һ��ȱʡ������";

		return ret;
	}
	
	public static String noActions( WorkflowProcess process, Activity act)
	{
		String ret =null;

		if( act.getTset_().getTools().size() == 0)
			ret =act.getId()+"->"+act.getName()+"�ڵ㣬������������һ��action";

		return ret;
	}

}
