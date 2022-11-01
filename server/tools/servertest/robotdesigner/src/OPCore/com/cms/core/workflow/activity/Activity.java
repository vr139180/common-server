package com.cms.core.workflow.activity;

import java.io.Serializable;
import java.util.Map;

import com.cms.core.*;
import com.cms.core.workflow.*;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: Activity.java,v 1.6 2004/10/10 09:17:15 york Exp $
 */
public class Activity extends AbstractElement implements Serializable
{
	public static int START = 1;
	public static int NORMAL = 0;
	
	private ToolSet tset_ =new ToolSet();

	public Activity(String id, String name)
	{
		super(id, name);
	}
	/**
	 * 获取活动类型
	 * @return
	 */
	public int getActivityType()
	{
		int ret =NORMAL;
		Map mp =getExtendedAttributes();
		String m =( String)mp.get( "activity.module");
		if( m != null)
		{
			if( m.equals( "S"))
				ret =START;
		}

		return ret;
	}

	/**
	 * @param i
	 */
	public void setActivityType(int i)
	{
		Map mp =getExtendedAttributes();
		if( i == START)
		{
			mp.put( "activity.module", "S");
			mp.put( "canStart", "Y");
		}
		else
		{
			mp.put( "activity.module", "N");
			mp.put( "canStart", "N");
		}
	}
	
	public void initDefaultProperty()
	{
		setActivityType( NORMAL);
	}
	
	public ToolSet getTset_() {
		return tset_;
	}
	
	
}
