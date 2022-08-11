package com.cms.designer.manager;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.Action;
import javax.swing.JButton;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.QName;
import org.dom4j.io.SAXReader;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEMenuBar;
import com.cms.designer.swingui.OBEToolBar;
import com.cms.designer.util.ConstValue;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 * $Id: ToolbarManager.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class ToolbarManager implements PropertyNames
{
	public static String barConfig = WorkSpaceManager.CONFIG;
	private OBEToolBar toolbar;
	private OBEDesigner parent;
	private File configFile;
	
	private String[] menubarArray={
			ConstValue.CoreFileNew,
			ConstValue.CoreFileOpen,
			ConstValue.CoreFileSave,
			ConstValue.CoreEditCut,
			ConstValue.CoreEditCopy,
			ConstValue.CoreEditPaste,
			
			ConstValue.CoreViewZoomin,
			ConstValue.CoreViewZoomout,
			ConstValue.CoreEditSelectAll,
			ConstValue.CoreEditSelectNone};
	
	public ToolbarManager(OBEDesigner parent)
	{
		this.parent = parent;
		toolbar =new OBEToolBar( parent);
		
		//初始化系统按钮
		initSysTools();
		this.configFile =
			new File(System.getProperty("ide.home") + "/conf/" + barConfig);
	}
	
	/*
	 * 初始化IDE框架的工具条
	 */
	private void initSysTools()
	{
		File config =new File( System.getProperty( "ide.home")+"/conf/"+WorkSpaceManager.CONFIG);
		try
		{
			FileInputStream in =new FileInputStream( config);
			SAXReader reader = new SAXReader();
			Document document = reader.read( in);
			Element e = document.getRootElement();
			Element e1 =e.element( "ide.toolbar");
			if( e1 == null)	return;
			parse( e1, ButtonManager.IDE_TOOLBAR);

			in.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
	}
	
	public void parse( InputStream in, String name)
		throws IOException, DocumentException
	{
		SAXReader reader = new SAXReader();
		Document document = reader.read(in);

		Element rootElement = document.getRootElement();
		Element workPlatsElement = rootElement.element( "workPlats");
		List workPlats =
			workPlatsElement.elements(
				new QName( "workPlat", workPlatsElement.getNamespace()));

		for (int i = 0; i < workPlats.size(); i++)
		{
			Element platElement = (Element)workPlats.get(i);
			String platName = platElement.attributeValue( "name");
			if (platName.equals(name))
			{
				Element toolbarElement = platElement.element( "toolbarConfig");
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
				new QName( "buttonSet", toolbarElement.getNamespace()));

		for (int i = 0; i < elements.size(); i++)
		{
			Element buttonElement = (Element)elements.get(i);
			String buttonName = buttonElement.attributeValue(NAME);
			addButton( name, buttonName);
		}
	}

	/**
	 * 增加按钮到工具条
	 * @param toolbarName
	 * @param buttonName
	 */
	private void addButton(String toolbarName, String buttonName)
	{
		ButtonManager buttonManager = parent.getButtonManager();
		if (buttonName.equals(SEPARATOR))
		{
			toolbar.addSeparator();
		}
		else
		{
			toolbar.add( buttonManager.getByName(buttonName));
			toolbar.addButton( buttonName);
		}
	}
	
	public OBEToolBar getToolBar()
	{
		return toolbar;
	}

	public OBEToolBar getToolBar(String name)
	{
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
	
	/**
	 * 更新
	 * @param actions
	 */
	//change the default toolbar actions into the special actions for special module
	public void updateToolbarActions( Map actions)
	{
		ButtonManager manager =parent.getButtonManager();
		List f =toolbar.getButtons();
		JButton jb =null;
		for( int i =0; i<f.size(); ++i)
		{
			String n =( String)f.get( i);
			jb =manager.getByName( n);
			if( jb == null)	continue;

			if( actions.containsKey( n))
			{
				//用新的替代旧的
				jb.setAction( ( Action)actions.get( n));
			}
			else
			{
				//使用系统缺省的
				manager.setDefaultAction( n);
			}
		}
		manager.loadResources();
	}
		
	//change the default menubar actions into the special actions for special module
	public void updateMenubarActions( Map actions)
	{
		//refresh OBEMenuBar
		OBEMenuBar menubar =parent.getOBEMenuBar();
		String oneMenuName=null;
		AbstractButton oneMenu =null;
		for( int i =0; i<menubarArray.length; ++i)
		{
			oneMenuName =( String)menubarArray[i];
			oneMenu =menubar.getByName( oneMenuName);
			if( oneMenu == null) continue;

			if( actions.containsKey( oneMenuName))
			{
				oneMenu.setAction( ( Action)actions.get( oneMenuName));
			}
			else
			{
				menubar.setDefaultAction( oneMenuName);
			}
		}
		menubar.loadResources();
	}
}
