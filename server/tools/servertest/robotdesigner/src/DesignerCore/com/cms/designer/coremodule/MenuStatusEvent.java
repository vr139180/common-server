package com.cms.designer.coremodule;

/**
 * @author Administrator
 * 状态事件
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class MenuStatusEvent
{
	private String id =null;
	private Object event =null;
	
	public MenuStatusEvent( String id, Object evt)
	{
		this.id =id;
		event =evt;
	}
	/**
	 * @return
	 */
	public Object getEvent()
	{
		return event;
	}

	/**
	 * @param id
	 */
	public void setId(String id)
	{
		this.id = id;
	}

}
