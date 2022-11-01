package com.cms.designer.workflow;

import java.util.StringTokenizer;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class TreeNodeKey
{
	public final static String PROCESS_PRE ="wf_process=";

	private String processID ="";
	
	public TreeNodeKey(){}
	
	/**
	 * @return
	 */
	public String createKey()
	{
		StringBuffer buf =new StringBuffer();

		if( processID != null && !processID.equals( ""))
		{
			buf.append( PROCESS_PRE);
			buf.append( processID);
		}

		return buf.toString();
	}
	
	/**
	 * @param k
	 */
	public void analyee( String k)
	{
		processID ="";
		
		StringTokenizer st =new StringTokenizer( k, "#");
		while( st.hasMoreTokens())
		{
			String e1 =st.nextToken();
			dealElement( e1);
		}
	}

	/**
	 * 处理key元素
	 * @param t
	 */
	private void dealElement( String t)
	{
		String id ="";
		if( t.startsWith( PROCESS_PRE))
		{
			id =t.substring( PROCESS_PRE.length());
			if( id != null || id.length() > 0)
				processID =id;
		}
	}

	/**
	 * @return
	 */
	public String getProcessID()
	{
		return processID;
	}

	/**
	 * @param processID
	 */
	public void setProcessID(String processID)
	{
		this.processID = processID;
	}

}
