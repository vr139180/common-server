package com.cms.designer.swingui.menu;

import java.util.Map;

import javax.swing.Action;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.ConstValue;
import com.cms.designer.util.OBETreeData;
import com.cms.designer.workflow.WorkFlowMenuManager;

/**
 * @author Administrator
 * 项目管理区的弹出式菜单管理
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: OBETreePopupMenu.java,v 1.2 2004/08/12 02:47:37 jefferyd Exp $
 */
public class OBETreePopupMenu extends JPopupMenu
{
	private OBEDesigner parent =null;
	//节点信息
	private OBETreeNode node =null;
	private String type;
	private String key;

	//根据是否有菜单决定是否显示菜单
	private boolean showit =false;

	private JMenu newMenu;
	//项目菜单
	private JMenuItem newProjectItem;
	
	//过程
	private JMenuItem newProcessItem;

	private JMenuItem importItem;
	private JMenuItem exportItem;
	private JMenuItem deleteItem;
	/**
	 * @param parent
	 * @param node
	 */
	public OBETreePopupMenu( OBEDesigner parent, OBETreeNode node)
	{
		this.parent = parent;
		this.node = node;
		this.type = ((OBETreeData)node.getUserObject()).getType();

		init();
	}

	/**
	 * 根据类型初始化弹出式菜单
	 */
	private void init()
	{
		Map actions = parent.getActions();
		showit =true;

		if (type.equals(OBETreeNode.ROOT))
		{ 
			//初始化根菜单
			initRootPop(actions);
		}
		else if (type.equals(OBETreeNode.PROJECT))
		{
			//初始化工程菜单
			initProjectPop(actions);
		}
		else if( type.equals( OBETreeNode.MODULE))
		{
			//处理模块菜单
			dealModuleMenu();
		}

		loadResources();
	}
	
	/**
	 * 处理菜单
	 */
	private void dealModuleMenu()
	{
		String key =((OBETreeData)node.getUserObject()).getKey();
		ElementKey ek =new ElementKey();
		ek.analyze( key);
		OBEModuleStandard ms =OBEModuleManager.getInstance().getModuleManager().getFirstModule();
		if( ms == null) return;
		showit =ms.getPopupMenu( this, key);
	}

	/**
	 * 导入菜单资源
	 */
	private void loadResources()
	{
		if (newMenu != null)
			newMenu.setText(ResourceUtil.getRS( ConstValue.CoreFileNew));

		if (newProjectItem != null)
			newProjectItem.setText(ResourceUtil.getRS( ConstValue.ProjectNew));

		if (importItem != null)
			importItem.setText(ResourceUtil.getRS( ConstValue.ProjectImport));

		if (exportItem != null)
			exportItem.setText(ResourceUtil.getRS( ConstValue.ProjectExport));

		if (deleteItem != null)
			deleteItem.setText(ResourceUtil.getRS( ConstValue.ProjectDelete));

		if( this.newProcessItem != null)
		{
			String str =OBEModuleManager.getInstance().getModuleManager().getFirstModule().getOBEModule().getResource( WorkFlowMenuManager.ACT_NEWPROCESS);
			newProcessItem.setText( str);
		}
	}

	/**
	 * 初始化根节点
	 * @param actions
	 */
	private void initRootPop(Map actions)
	{
		newMenu = new JMenu();
		
		newProjectItem = newMenu.add( (Action)actions.get( ConstValue.ProjectNew));
		add(newMenu);
		
		addSeparator();
		//导入项目
		importItem = add((Action)actions.get( ConstValue.ProjectImport));
	}

	/**
	 * 初始化工程菜单
	 * @param actions
	 */
	private void initProjectPop( Map actions)
	{
		newProcessItem =add((Action)actions.get( WorkFlowMenuManager.ACT_NEWPROCESS));

		//导出工程
		exportItem = add((Action)actions.get( ConstValue.ProjectExport));
		addSeparator();
		deleteItem = add((Action)actions.get( ConstValue.ProjectDelete));
	}

	public boolean isShowit()
	{
		return showit;
	}
}
