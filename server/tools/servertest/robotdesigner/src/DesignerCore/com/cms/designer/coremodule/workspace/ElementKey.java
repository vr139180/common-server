package com.cms.designer.coremodule.workspace;

import java.util.StringTokenizer;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class ElementKey
{
	public final static String PROJECT_PRE ="project=";
	public final static String MODULETYPE_PRE ="moduletype=";
	public final static String MODULEID_PRE ="moduleid=";
	public final static String MODULEEXT_PRE ="moduleext=";
	public final static String MODULEEXTATT_PRE ="moduleextatt=";
	
	private String projectID ="";
	private String moduleType ="";
	private String moduleID ="";
	private String moduleExt ="";
	private String moduleExtAtt ="";
	
	public ElementKey()
	{
	}

	public ElementKey( String projectID)
	{
		this();
		this.projectID =projectID;
	}
	
	public ElementKey( String projectID, String moduleType, String moduleID)
	{
		this( projectID);
		this.moduleType =moduleType;
		this.moduleID =moduleID;
	}
	
	public ElementKey( String projectID, String moduleType, String moduleID, String moduleExt)
	{
		this( projectID, moduleType, moduleID);
		this.moduleExt =moduleExt;
	}
	
	/**
	 * 创建初一个key字符串
	 * @return
	 */
	public String createKey()
	{
		StringBuffer buf =new StringBuffer();
		boolean bfirst =true;
		if( projectID != null && !projectID.equals( ""))
		{
			buf.append( PROJECT_PRE);
			buf.append( projectID);
			bfirst =false;
		}
		
		if( moduleType != null && !moduleType.equals( ""))
		{
			if( !bfirst)
				buf.append( ":");
			buf.append( MODULETYPE_PRE);
			buf.append( moduleType);
			bfirst =false;
		}
		
		if( moduleID != null && !moduleID.equals( ""))
		{
			if( !bfirst)
				buf.append( ":");
			buf.append( MODULEID_PRE);
			buf.append( moduleID);
			bfirst =false;
		}

		if( moduleExt != null && !moduleExt.equals( ""))
		{
			if( !bfirst)
				buf.append( ":");
			buf.append( MODULEEXT_PRE);
			buf.append( moduleExt);
			bfirst =false;
		}

		if( moduleExtAtt != null && !moduleExtAtt.equals( ""))
		{
			if( !bfirst)
				buf.append( ":");
			buf.append( MODULEEXTATT_PRE);
			buf.append( moduleExtAtt);
			bfirst =false;
		}

		return buf.toString();
	}
	
	/**
	 * 分析制定的字符创
	 * @param k
	 */
	public void analyze( String k)
	{
		if( k == null)	return;
		projectID ="";
		moduleType ="";
		moduleID ="";
		moduleExt ="";
		moduleExtAtt ="";
		
		StringTokenizer st =new StringTokenizer( k, ":");
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
		if( t.startsWith( PROJECT_PRE))
		{
			id =t.substring( PROJECT_PRE.length());
			if( id != null && id.length() > 0)
				projectID =id;
		}
		else if( t.startsWith( MODULETYPE_PRE))
		{
			id =t.substring( MODULETYPE_PRE.length());
			if( id != null && id.length() > 0)
				moduleType =id;
		}
		else if( t.startsWith( MODULEID_PRE))
		{
			id =t.substring( MODULEID_PRE.length());
			if( id != null && id.length() > 0)
				moduleID =id;
		}
		else if( t.startsWith( MODULEEXT_PRE))
		{
			id =t.substring( MODULEEXT_PRE.length());
			if( id != null && id.length() > 0)
				moduleExt =id;
		}
		else if( t.startsWith( MODULEEXTATT_PRE))
		{
			id =t.substring( MODULEEXTATT_PRE.length());
			if( id != null && id.length() > 0)
				moduleExtAtt =id;
		}
	}
	
	/**
	 * @return
	 */
	public String getModuleExt()
	{
		return moduleExt;
	}

	/**
	 * @param moduleExt
	 */
	public void setModuleExt(String moduleExt)
	{
		this.moduleExt = moduleExt;
	}

	/**
	 * @return
	 */
	public String getModuleID()
	{
		return moduleID;
	}

	/**
	 * @param moduleID
	 */
	public void setModuleID(String moduleID)
	{
		this.moduleID = moduleID;
	}

	/**
	 * @return
	 */
	public String getModuleType()
	{
		return moduleType;
	}

	/**
	 * @param moduleType
	 */
	public void setModuleType(String moduleType)
	{
		this.moduleType = moduleType;
	}

	/**
	 * @return
	 */
	public String getProjectID()
	{
		return projectID;
	}

	/**
	 * @param projectID
	 */
	public void setProjectID(String projectID)
	{
		this.projectID = projectID;
	}

	/**
	 * @return
	 */
	public String getModuleExtAtt()
	{
		return moduleExtAtt;
	}

	/**
	 * @param moudleExtAtt
	 */
	public void setModuleExtAtt(String moduleExtAtt)
	{
		this.moduleExtAtt = moduleExtAtt;
	}

}
