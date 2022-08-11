package com.cms.designer.coremodule;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;

import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JPopupMenu;

import com.anthonyeden.lib.util.IconManager;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.beacon.EventObservable;
import com.cms.designer.coremodule.workspace.OBEModuleData;
import com.cms.designer.swingui.OBEComponentBar;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.util.ConstValue;

/**
 * @author Administrator
 * 各个模块的主入口
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEModuleStandard.java,v 1.4 2004/08/23 02:36:10 york Exp $
 */
public abstract class OBEModuleStandard
{
	//图片资源
	public static IconManager iconManager = com.cms.designer.util.ResourceUtil.getIconManager();

	private OBEDesigner parent = null;
	//模块描述
	private OBEModule descriptions =null;
	
	private final Long lockOBJ =new Long( 0);
	//存放相关的模块数据
	private final HashMap moduleDatas =new HashMap();
	private long uuid =0;
	
	//存放工具条按钮
	private OBEComponentBar toolbar =null;
	//存放系统菜单动作实现
	private final HashMap ideActions =new HashMap();
	
	private EventObservable beacon;
	private static EventObservable defaultBeacon;
	
	/**
	 * @param m
	 * @param p
	 */
	public OBEModuleStandard( OBEModule m, OBEDesigner p)
	{
		if( m == null)
			throw new NullPointerException( "OBEModule can't null");
		descriptions =m;
		parent =p;
		//初始化工具条
		toolbar =new OBEComponentBar( parent);
	}

	//获取模块菜单
	public abstract JMenu getModuleMenu();
	//根据标示信息获取弹出式菜单
	public abstract boolean getPopupMenu( JPopupMenu root, String moduleExtAtt);
	//获取空的模块数据描述
	public abstract OBEModuleData newModuleData( String projectID, String moduleID);
	//获取模块edit窗口
	public abstract OBEInternalFrame newModuleFrame( String key);
	//判断是否和窗口相关联
	public abstract boolean canOpenView( String moduleExtAtt);
	//加载指定路径的模块信息
	public abstract OBEModuleData loadModuleData( String path, String projectID);

	//获取工具条信息
	public final OBEComponentBar getModuleToolbar()
	{
		return toolbar.getToolbar();
	}

	/**
	 * 增加一个模块数据
	 * @param md
	 * @return
	 */
	public final synchronized boolean addModuleData( OBEModuleData md)
	{
		boolean ret =true;
		//创建uuid
		synchronized( lockOBJ)
		{
			++uuid;
			md.setUuid( Long.toString( uuid));
			moduleDatas.put( md.getUuid(), md);
		}
		
		return ret;
	}
	
	/**
	 * 删除模块数据
	 * @param uuid
	 */
	public final synchronized void removeModuleData( String uuid)
	{
		moduleDatas.remove( uuid);
	}
	
	/**
	 * 根据uuid获取数据
	 * @param uuid
	 * @return
	 */
	public final OBEModuleData getModuleData( String uuid)
	{
		return ( OBEModuleData)moduleDatas.get( uuid);
	}
	
	protected final Iterator getModuleUuid(){
		return moduleDatas.keySet().iterator();
	}

	//获取缺省的节点图标
	public ImageIcon getDefaultNodeImage()
	{
		return null;
	}
	
	//获取制定名称的节点图标
	public ImageIcon getNodeImage(int type)
	{
		return null;
	}

	public final OBEDesigner getParent()
	{
		return parent;
	}
	
	public final OBEModule getOBEModule()
	{
		return descriptions;
	}

	/**
	 * 注册用到的按钮
	 * @param name
	 * @param icon
	 * @param selectedIcon
	 * @param tooltip
	 * @return
	 */
	public final JButton registerToolbarButton(
		String name,
		String icon,
		String selectedIcon,
		String tooltip)
	{
		name =name.toLowerCase();
		if( toolbar.containButton( name))
			return ( JButton)toolbar.getButton( name);
		
		JButton button = new JButton();
		button.setActionCommand( name);
		button.addMouseListener( new ComponentButtonAction());
		button.setIcon( iconManager.getIcon(icon));
		button.setSelectedIcon( iconManager.getIcon(selectedIcon));
		button.setToolTipText(tooltip);
		
		//增加到工具条
		toolbar.addButton( name, button);
		
		return button;
	}
	
	public final void registerSeparator()
	{
		toolbar.addSeparator();
	}
	
	public final JButton registerToolbarButton(
		Action action,
		String name,
		String icon,
		String selectedIcon,
		String tooltip)
	{
		name =name.toLowerCase();
		if( toolbar.containButton( name))
			return ( JButton)toolbar.getButton( name);
			
		JButton button = new JButton( action);
		button.setIcon( iconManager.getIcon(icon));
		button.setSelectedIcon( iconManager.getIcon(selectedIcon));
		button.setToolTipText(tooltip);
			
		//增加到工具条
		toolbar.addButton( name, button);
			
		return button;
	}
	
	/**
	 * 注册系统的动作
	 * @param ideact
	 * @param action
	 * @return
	 */
	public final OBEStandardAction registerIDEAction( String ideact, OBEStandardAction action)
	{
		OBEStandardAction ret =null;
		if( !ConstValue.containSystemMenu( ideact))
			return ret;
		if( ideActions.containsKey( ideact))
			ret =( OBEStandardAction)ideActions.get( ideact);
		else
		{
			ideActions.put( ideact, action);
			ret =action;
		}
		
		return ret;
	}
	
	/**
	 * 获取系统动作
	 * @param ide
	 * @return
	 */
	public final OBEStandardAction getIDEAction( String ide)
	{
		return ( OBEStandardAction)ideActions.get( ide);
	}
	
	public final Map getIDEAction()
	{
		return ideActions;
	}

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	final class ComponentButtonAction extends MouseAdapter
	{
		public void mousePressed(MouseEvent e)
		{
			if (e.getButton() == MouseEvent.BUTTON1)
			{
				JButton button = (JButton)e.getSource();
				String key = button.getActionCommand();
				getModuleToolbar().modifyBorder( key);
			}
		}
	}

	/**
	 * @return Returns the beacon.
	 */
	public EventObservable getBeacon()
	{
		return beacon;
	}
	/**
	 * @param beacon The beacon to set.
	 */
	public void setBeacon( EventObservable beacon)
	{
		this.beacon = beacon;
	}
	/**
	 * @return Returns the defaultBeacon.
	 */
	public EventObservable getDefaultBeacon()
	{
		return defaultBeacon;
	}
	/**
	 * @param defaultBeacon The defaultBeacon to set.
	 */
	public void setDefaultBeacon( EventObservable dBeacon)
	{
		defaultBeacon = dBeacon;
	}
	/**
	 * @return Returns the descriptions.
	 */
	public OBEModule getDescriptions()
	{
		return descriptions;
	}
}
