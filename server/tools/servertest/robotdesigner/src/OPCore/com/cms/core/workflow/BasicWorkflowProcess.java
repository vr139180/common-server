package com.cms.core.workflow;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.util.ExtendedAttributeNames;
import com.cms.designer.workflow.OBEGraphWorkflow;

public class BasicWorkflowProcess extends AbstractWorkflowProcess
{
	public BasicWorkflowProcess( String id, String name)
	{
		super( id, name);
	}

	/**
	 * ��ʼ�����̶��壬���������յ�
	 */
	public void initBasicWorkflowProcess()
	{
		//����ȱʡ����ʼ�ڵ�
		Activity start = new Activity( getNextActivityId(), ResourceUtil.getRS( "default.start"));//��ʼ
		start.setActivityType( Activity.START);

		getActivities().add( start);

		//����ȱʡλ��
		Map mp = start.getExtendedAttributes();
		mp.put( ExtendedAttributeNames.BOUNDS_X, "50");
		mp.put( ExtendedAttributeNames.BOUNDS_Y, "50");
		mp.put( ExtendedAttributeNames.BOUNDS_WIDTH, "" + OBEGraphWorkflow.X_WIDTH);
		mp.put( ExtendedAttributeNames.BOUNDS_HEIGHT, "" + OBEGraphWorkflow.Y_WIDTH);
	}
	
	public List getTransitionsFrom( String id)
	{
		List ret =new ArrayList();
		
		Iterator ff =this.getTransitions().iterator();
		while( ff.hasNext())
		{
			Transition t =(Transition)ff.next();
			if( t.getFrom().equals( id))
				ret.add( t);
		}
		
		return ret;
	}
}
