package com.cms.designer.manager;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import javax.swing.JButton;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.QName;
import org.dom4j.io.SAXReader;

import com.cms.designer.swingui.OBEComponentBar;
import com.cms.designer.swingui.OBEDesigner;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: ComponentBarManager.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class ComponentBarManager implements PropertyNames
{
	public static String barConfig = WorkSpaceManager.CONFIG;

	private OBEComponentBar toolbar;
	private OBEDesigner parent;
	private File configFile;

	public ComponentBarManager(OBEDesigner parent)
	{
		this.parent = parent;
		this.configFile =
			new File(System.getProperty("ide.home") + "/conf/" + barConfig);
		
		this.toolbar = new OBEComponentBar( parent);
	}

	public void parse(InputStream in, String name)
		throws IOException, DocumentException
	{
		SAXReader reader = new SAXReader();
		Document document = reader.read(in);

		Element rootElement = document.getRootElement();
		Element workPlatsElement = rootElement.element(WORKPLATS);
		List workPlats =
			workPlatsElement.elements(
				new QName(WORKPLAT, workPlatsElement.getNamespace()));

		for (int i = 0; i < workPlats.size(); i++)
		{
			Element platElement = (Element)workPlats.get(i);
			String platName = platElement.attributeValue(NAME);
			if (platName.equals(name))
			{
				Element toolbarElement =
					platElement.element(COMPONENTBARCONFIG);
				parse(toolbarElement, name);
				break;
			}
		}
		in.close();
	}

	private void parse(Element toolbarElement, String name)
	{
		List elements =
			toolbarElement.elements(
				new QName(BUTTONSET, toolbarElement.getNamespace()));

		for (int i = 0; i < elements.size(); i++)
		{
			Element buttonElement = (Element)elements.get(i);
			String buttonName = buttonElement.attributeValue(NAME);
			if( buttonName != null)
				addButton(name, buttonName);
		}
	}

	private void addButton(String toolbarName, String buttonName)
	{
		ButtonManager buttonManager = parent.getButtonManager();
		JButton button = buttonManager.getByName(buttonName);
		if( button != null)
		{
			toolbar.add(button);
			toolbar.getHm().put(buttonName, button);
		}
	}

	public OBEComponentBar getToolBar()
	{
		return toolbar;
	}
	
	public OBEComponentBar getToolBar(String name)
	{
		toolbar.removeAll();
		try
		{
			parse(new FileInputStream(configFile), name);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return toolbar;
	}
	
}
