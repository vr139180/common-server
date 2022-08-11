package com.cms.designer.util;

/**
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class OBETreeData
{
	private String name;
	private String type;
	private String iconName;
	private String key ="";
	
	public OBETreeData(String name, String type, String iconName)
	{
		this.name = name;
		this.type = type;
		this.iconName = iconName;
	}

	public OBETreeData(String name, String type, String iconName, String key)
	{
		this( name, type, iconName);
		this.key =key;
	}

	public String getName()
	{
		return name;
	}
	public String getType()
	{
		return type;
	}
	public void setName(String string)
	{
		name = string;
	}
	public void setType(String string)
	{
		type = string;
	}

	public String getIconName()
	{
		return iconName;
	}

	public void setIconName(String string)
	{
		iconName = string;
	}

	public String toString()
	{
		return key;
	}
	
	/**
	 * @return
	 */
	public String getKey()
	{
		return key;
	}

	/**
	 * @param key
	 */
	public void setKey(String key)
	{
		this.key = key;
	}

}
