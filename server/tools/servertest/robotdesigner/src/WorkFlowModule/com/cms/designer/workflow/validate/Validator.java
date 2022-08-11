package com.cms.designer.workflow.validate;

import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.validate.Validate;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.validate.rule.WorkflowActivityValidate;
import com.cms.designer.workflow.validate.rule.WorkflowProcessValidate;
import com.cms.designer.workflow.validate.rule.WorkflowTransitionValidate;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class Validator implements Validate
{
	private WorkflowPackage wp =null;
	private WorkflowProcess process =null;
	private WorkflowModuleData data = null;
	private ProjectData pd = null ;
	
	private Map errMap =new HashMap();
	
	/**
	 * 20040811 york 修改 增加了 WorkflowModuleData data 参数 将在WorkflowProcessValidate中使用
	 * @param data
	 * @param wp
	 * @param process
	 */
	public Validator(ProjectData pd ,WorkflowModuleData data, WorkflowPackage wp, WorkflowProcess process)
	{
		if( pd == null || data == null || wp == null || process == null)
			throw new NullPointerException( "can't null ,this value");
		this.wp =wp;
		this.process =process;
		this.data = data;
		this.pd = pd;
	}

	public boolean validate()
	{
		boolean ret =true;
		
		//验证package
		List ll =null;
		
		String error =null;
		
		//验证process
		ll =( List)errMap.get( "process");
		if( ll == null)
		{
			ll =new ArrayList();
			errMap.put( "process", ll);
		}
		
		if( process == null)
		{
			if( !WorkflowProcessValidate.validateProcess(pd, wp, ll))
				ret =false;
		}
		else
		{
			if( !WorkflowProcessValidate.validateProcess(pd, wp, process, ll))
				ret =false;
			
		}
		
		//验证节点
		ll =( List)errMap.get( "activity");
		if( ll == null)
		{
			ll =new ArrayList();
			errMap.put( "activity", ll);
		}
		if( process == null)
		{
			Iterator ff =wp.getWorkflowProcesses().iterator();
			while( ff.hasNext())
			{
				WorkflowProcess pp =( WorkflowProcess)ff.next();
				if( !WorkflowActivityValidate.validateActivity(pd, data,pp, ll))
					ret =false;
			}
		}
		else
		{
			if( !WorkflowActivityValidate.validateActivity(pd,data, process, ll))
				ret =false;
		}
		
		//验证转移
		ll =( List)errMap.get( "transition");
		if( ll == null)
		{
			ll =new ArrayList();
			errMap.put( "transition", ll);
		}
		if( process == null)
		{
			Iterator ff =wp.getWorkflowProcesses().iterator();
			while( ff.hasNext())
			{
				WorkflowProcess pp =( WorkflowProcess)ff.next();
				if( !WorkflowTransitionValidate.validateTransition(pd, data,pp, ll))
					ret =false;
			}
		}
		else
		{
			if( !WorkflowTransitionValidate.validateTransition(pd, data, process, ll))
				ret =false;
		}

		return ret;
	}
	
	public String getFormatMsg()
	{
		String style1 ="<font size=\"+1\" color=\"#0000FF\"><strong>";
		String style2 ="</strong></font><br>";
		String style3 ="<br>";
		
		StringBuffer buffer =new StringBuffer( "");
		
		Date nowTime =new Date();
		DateFormat df =DateFormat.getDateInstance( DateFormat.MEDIUM);
		buffer.append( df.format( nowTime));
		buffer.append( style3);
		
		List ll =null;
		
		ll =( List)errMap.get( "process");
		if( ll != null && ll.size() > 0)
		{
			//构造process信息
			buffer.append( style1);
			buffer.append( "流程的错误");
			buffer.append( style2);
			Iterator ff =ll.iterator();
			while( ff.hasNext())
			{
				buffer.append( ( String)ff.next());
				buffer.append( style3);
			}
		}

		ll =( List)errMap.get( "activity");
		if( ll != null && ll.size() > 0)
		{
			//构造process信息
			buffer.append( style1);
			buffer.append( "节点的错误");
			buffer.append( style2);
			Iterator ff =ll.iterator();
			while( ff.hasNext())
			{
				buffer.append( ( String)ff.next());
				buffer.append( style3);
			}
		}

		ll =( List)errMap.get( "transition");
		if( ll != null && ll.size() > 0)
		{
			//构造process信息
			buffer.append( style1);
			buffer.append( "转移的错误");
			buffer.append( style2);
			Iterator ff =ll.iterator();
			while( ff.hasNext())
			{
				buffer.append( ( String)ff.next());
				buffer.append( style3);
			}
		}

		return buffer.toString();
	}
}
