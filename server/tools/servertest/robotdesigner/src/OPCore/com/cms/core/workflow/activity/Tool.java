package com.cms.core.workflow.activity;

import java.io.File;
import java.util.*;

import com.cms.core.workflow.*;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ProjectData;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

public class Tool implements Implementation
{
	private String name;

	private Map	params =new HashMap();
	
	public Tool( String n)
	{
		name =n;
	}

	public String toString()
	{
		return name==null?"no action":name;
	}
	
	/**
	 * @return Returns the params.
	 */
	public Map getParams()
	{
		return params;
	}
	
	/**
	 * @return Returns the name.
	 */
	public String getName()
	{
		return name;
	}
	/**
	 * @param name The name to set.
	 */
	public void setName( String name)
	{
		this.name = name;
	}
	
	private static Map s_allactions =new HashMap();
	private static List<String> s_lists =new ArrayList<String>();
	
	public static List<String> getAllActions()
	{
		return s_lists;
	}
	
	public static Map getParamsOfAction( String name)
	{
		Map ret =(Map)s_allactions.get( name);
		return ret;
	}

	public static String ACTIONS_FILE = "support_action.properties";

	static
	{
		File actionFile = new File(System.getProperty("ide.home") + "/conf/" + ACTIONS_FILE);

		//load action defined
		try
		{
			SAXBuilder builder =new SAXBuilder();
			Document doc =builder.build( actionFile);
			Element root =doc.getRootElement();
			Iterator f =root.getChildren( "action").iterator();
			while( f.hasNext())
			{
				Element e =( Element)f.next();

				String actionName =e.getAttributeValue( "name");

				actionName = actionName.toLowerCase().trim();
				s_lists.add( actionName);
				Map pms = new HashMap();
				s_allactions.put( actionName, pms);

				Iterator pf = e.getChildren( "pm").iterator();
				while( pf.hasNext())
				{
					Element epms =( Element)pf.next();
					String n = epms.getAttributeValue( "n").toLowerCase().trim();
					String v = epms.getAttributeValue( "v").toLowerCase().trim();

					pms.put( n, v);
				}
			}
		}
		catch( Exception e1){
			e1.printStackTrace();
		}
	}
	

}
