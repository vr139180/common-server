package com.cms.designer.swingui;
import java.awt.Component;
import java.awt.Event;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.menu.InternalFrameButton;
import com.cms.designer.swingui.menu.WindowMenu;
import com.cms.designer.swingui.panel.InternalFramePanel;
import com.cms.designer.util.ConstValue;

/**
 * @author Administrator
 * 框架菜单
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEMenuBar.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class OBEMenuBar extends JMenuBar
{
	private final static String _tabBarItem ="menuitem.tabBarItem";
	private final static String _toolBarItem ="menuitem.toolBarItem";
	private final static String _componentBarItem ="menuitem.componentBarItem";
	private final static String _statusBarItem ="menuitem.statusBarItem";

	//图标管理
	public IconManager iconManager =com.cms.designer.util.ResourceUtil.getIconManager();
	//占位图
	public final Icon tm = iconManager.getIcon("tm.gif");

	private OBEDesigner parent =null;
	
	private HashMap menuMap;
	//窗口控制按钮
	private WindowMenu windowMenu;
	
	//文件菜单
	private JMenu fileMenu;
	private JMenuItem newMenuItem;
	private JMenuItem openMenuItem;
	private JMenuItem closeMenuItem;
//	private JMenuItem revertMenuItem;

	private JMenuItem saveMenuItem;
	//private JMenuItem printMenuItem;
	private JMenuItem quitMenuItem;

	//编辑菜单
	private JMenu editMenu;
//	private JMenuItem undoMenuItem;
//	private JMenuItem redoMenuItem;
	private JMenuItem cutMenuItem;
	private JMenuItem copyMenuItem;
	private JMenuItem pasteMenuItem;
	private JMenuItem selectAllMenuItem;
	private JMenuItem selectNoneMenuItem;

	//视图菜单
	private JMenu viewMenu;
	private JCheckBoxMenuItem tabBarItem, toolBarItem, statusBarItem;

//	private JMenu zoomMenu;
	private JMenuItem magnifyMenuItem, minifyMenuItem;

	//帮助菜单
	private JMenu helpMenu;
	private JMenuItem topicMenuItem;
	private JMenuItem aboutMenuItem;

	//操作internalframe的菜单
	private InternalFramePanel buttonPanel;
	public InternalFrameButton minimizeButton;
	public InternalFrameButton revertButton;
	public InternalFrameButton closeButton;
	
	private ActionListener actionlnsr =new ActionListener()
	{
		public void actionPerformed(ActionEvent e)
		{
			String cmd =e.getActionCommand();
			if( cmd == null)	return;
			if( cmd.equals( _tabBarItem))
			{
				boolean selected = tabBarItem.getState();
				tabBarItem.setState(selected);
				getDesignerParent().setTabBarVisiable(selected);
				
				int maxWidth = getDesignerParent().getTabSplit().getWidth();
				if (selected)
					getDesignerParent().getTabSplit().setDividerLocation(600);
				else
					getDesignerParent().getTabSplit().setDividerLocation(
						maxWidth);
			}
			else if( cmd.equals( _toolBarItem))
			{
				boolean selected = toolBarItem.getState();
				toolBarItem.setState(selected);
				getDesignerParent().getToolBar().setVisible(selected);
			}
			/*
			else if( cmd.equals( _componentBarItem))
			{
				boolean selected = componentBarItem.getState();
				componentBarItem.setState(selected);
				getDesignerParent().getComponentbarManager().getToolBar().setVisible(selected);
			}
			*/
			else if( cmd.equals( _statusBarItem))
			{
				boolean selected = statusBarItem.getState();
				statusBarItem.setState(selected);
				getDesignerParent().setStatusVisiable(selected);
			}
		}
	};

	public OBEMenuBar(OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}
	
	/**
	 * load the text on the MenuItem
	 *
	 */
	public void loadResources()
	{
		fileMenu.setText(ResourceUtil.getRS("window.file"));
		fileMenu.setMnemonic('f');

		newMenuItem.setText(ResourceUtil.getRS("window.new"));
		newMenuItem.setIcon(iconManager.getIcon("new_16.gif"));
		newMenuItem.setMnemonic('n');
		newMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_N, Event.CTRL_MASK));

		openMenuItem.setIcon(iconManager.getIcon("folder_16.gif"));
		openMenuItem.setText(ResourceUtil.getRS("window.open"));
		openMenuItem.setMnemonic('o');
		openMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_O, Event.CTRL_MASK));

		closeMenuItem.setIcon(tm);
		closeMenuItem.setText(ResourceUtil.getRS("window.close"));
		closeMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_W, Event.CTRL_MASK));
		closeMenuItem.setEnabled(false);
		
		saveMenuItem.setIcon(iconManager.getIcon("save_16.gif"));
		saveMenuItem.setText(ResourceUtil.getRS("window.save"));
		saveMenuItem.setMnemonic('s');
		saveMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_S, Event.CTRL_MASK));
		saveMenuItem.setEnabled(false);/*
		
		printMenuItem.setIcon(tm);
		printMenuItem.setText(ResourceUtil.getRS( ConstValue.CoreFilePrint));
		printMenuItem.setMnemonic('p');
		printMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_P, Event.CTRL_MASK));
		printMenuItem.setEnabled(false);*/
		
		quitMenuItem.setIcon(tm);
		quitMenuItem.setText(ResourceUtil.getRS("window.quit"));
		quitMenuItem.setMnemonic('q');
		quitMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_Q, Event.CTRL_MASK));

		editMenu.setText(ResourceUtil.getRS("window.edit"));
		editMenu.setMnemonic('e');

		cutMenuItem.setIcon(iconManager.getIcon("cut_16.gif"));
		cutMenuItem.setText(ResourceUtil.getRS("window.cut"));
		cutMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_X, Event.CTRL_MASK));
		cutMenuItem.setEnabled(false);
		
		copyMenuItem.setIcon(iconManager.getIcon("copy_16.gif"));
		copyMenuItem.setText(ResourceUtil.getRS("window.copy"));
		copyMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_C, Event.CTRL_MASK));
		copyMenuItem.setEnabled(false);
		
		pasteMenuItem.setIcon(iconManager.getIcon("paste_16.gif"));
		pasteMenuItem.setText(ResourceUtil.getRS("window.paste"));
		pasteMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_V, Event.CTRL_MASK));
		pasteMenuItem.setEnabled(false);
		
		selectAllMenuItem.setIcon(tm);
		selectAllMenuItem.setText(ResourceUtil.getRS("window.selectAll"));
		selectAllMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(KeyEvent.VK_A, Event.CTRL_MASK));
		selectAllMenuItem.setEnabled(false);
		
		selectNoneMenuItem.setIcon(tm);
		selectNoneMenuItem.setText(ResourceUtil.getRS("window.selectNone"));
		selectNoneMenuItem.setAccelerator(
			KeyStroke.getKeyStroke(
				KeyEvent.VK_A,
				Event.CTRL_MASK + Event.SHIFT_MASK));
		selectNoneMenuItem.setEnabled(false);
		
//		zoomMenu.setIcon(tm);
//		zoomMenu.setText(ResourceUtil.getRS("window.zoom"));

		viewMenu.setText(ResourceUtil.getRS("view.view"));
		viewMenu.setMnemonic('v');
		
		tabBarItem.setIcon(tm);
		tabBarItem.setText(ResourceUtil.getRS("view.tabBar"));
		toolBarItem.setIcon(tm);
		toolBarItem.setText(ResourceUtil.getRS("view.toolBar"));
		statusBarItem.setIcon(tm);
		statusBarItem.setText(ResourceUtil.getRS("view.statusBar"));

		magnifyMenuItem.setText(ResourceUtil.getRS("view.magnify"));
		magnifyMenuItem.setIcon(iconManager.getIcon("zoomIn1.gif"));
		minifyMenuItem.setText(ResourceUtil.getRS("view.minify"));
		minifyMenuItem.setIcon(iconManager.getIcon("zoomOut1.gif"));
		magnifyMenuItem.setEnabled(false);
		minifyMenuItem.setEnabled(false);
		
		windowMenu.setText(ResourceUtil.getRS("windowMenu.window"));
		windowMenu.setMnemonic('w');

		helpMenu.setText(ResourceUtil.getRS("help.help"));
		helpMenu.setMnemonic('h');

		topicMenuItem.setIcon(iconManager.getIcon("help.gif"));
		topicMenuItem.setText(ResourceUtil.getRS("help.topic"));
		topicMenuItem.setMnemonic('h');
		aboutMenuItem.setIcon(tm);
		aboutMenuItem.setText(ResourceUtil.getRS("help.about"));
		aboutMenuItem.setMnemonic('a');
	}

	/**
	 * 初始化菜单
	 */
	private void init()
	{
		menuMap=new HashMap();
		Map actions = parent.getActions();

		/*文件菜单
		fileMenu = new JMenu();
		newMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreFileNew));
		openMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreFileOpen));
		saveMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreFileSave));
		closeMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreFileClose));
		fileMenu.addSeparator();
		printMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreFilePrint));
		fileMenu.addSeparator();
		quitMenuItem = fileMenu.add((Action)actions.get( ConstValue.CoreSysQuit));
		add(fileMenu);*/
		fileMenu = new JMenu();
		newMenuItem = createMenuItem(fileMenu,ConstValue.CoreFileNew,(Action)actions.get( ConstValue.CoreFileNew));
		openMenuItem = createMenuItem(fileMenu,ConstValue.CoreFileOpen,(Action)actions.get( ConstValue.CoreFileOpen));
		saveMenuItem = createMenuItem(fileMenu,ConstValue.CoreFileSave,(Action)actions.get( ConstValue.CoreFileSave));
		closeMenuItem = createMenuItem(fileMenu,ConstValue.CoreFileClose,(Action)actions.get( ConstValue.CoreFileClose));
		fileMenu.addSeparator();/*
		printMenuItem = createMenuItem(fileMenu,ConstValue.CoreFilePrint,(Action)actions.get( ConstValue.CoreFilePrint));
		fileMenu.addSeparator();*/
		quitMenuItem = createMenuItem(fileMenu,ConstValue.CoreSysQuit,(Action)actions.get( ConstValue.CoreSysQuit));
		add(fileMenu);

		/*编辑操作
		editMenu = new JMenu();
		cutMenuItem = editMenu.add((Action)actions.get( ConstValue.CoreEditCut));
		copyMenuItem = editMenu.add((Action)actions.get( ConstValue.CoreEditCopy));
		pasteMenuItem = editMenu.add((Action)actions.get( ConstValue.CoreEditPaste));
		editMenu.addSeparator();
		selectAllMenuItem = editMenu.add((Action)actions.get( ConstValue.CoreEditSelectAll));
		selectNoneMenuItem =
			editMenu.add((Action)actions.get( ConstValue.CoreEditSelectNone));
		add(editMenu);*/
		editMenu = new JMenu();
		cutMenuItem = createMenuItem(editMenu,ConstValue.CoreEditCut,(Action)actions.get( ConstValue.CoreEditCut));
		copyMenuItem = createMenuItem(editMenu,ConstValue.CoreEditCopy,(Action)actions.get( ConstValue.CoreEditCopy));
		pasteMenuItem = createMenuItem(editMenu,ConstValue.CoreEditPaste,(Action)actions.get( ConstValue.CoreEditPaste));
		editMenu.addSeparator();
		selectAllMenuItem = createMenuItem(editMenu,ConstValue.CoreEditSelectAll,(Action)actions.get( ConstValue.CoreEditSelectAll));
		selectNoneMenuItem =
			createMenuItem(editMenu,ConstValue.CoreEditSelectNone,(Action)actions.get( ConstValue.CoreEditSelectNone));
		add(editMenu);

		//视图菜单
		viewMenu = new JMenu();
		//工作台
		tabBarItem = new JCheckBoxMenuItem();
		tabBarItem.setState(true);
		tabBarItem.setActionCommand( _tabBarItem);
		tabBarItem.addActionListener( actionlnsr);
		viewMenu.add(tabBarItem);

		//工具条
		toolBarItem = new JCheckBoxMenuItem();
		toolBarItem.setState(true);
		toolBarItem.setActionCommand( _toolBarItem);
		toolBarItem.addActionListener( actionlnsr);
		viewMenu.add(toolBarItem);

		//状态条
		statusBarItem = new JCheckBoxMenuItem();
		statusBarItem.setState(true);
		statusBarItem.setActionCommand( _statusBarItem);
		statusBarItem.addActionListener( actionlnsr);
		viewMenu.add(statusBarItem);

		viewMenu.addSeparator();

//		zoomMenu = new ZoomMenu(parent);
//		viewMenu.add(zoomMenu);
		/*
		magnifyMenuItem =viewMenu.add((Action)actions.get(ConstValue.CoreViewZoomin));
		minifyMenuItem =viewMenu.add((Action)actions.get(ConstValue.CoreViewZoomout));*/
		magnifyMenuItem = createMenuItem(viewMenu,ConstValue.CoreViewZoomin,(Action)actions.get( ConstValue.CoreViewZoomin));
		minifyMenuItem = createMenuItem(viewMenu,ConstValue.CoreViewZoomout,(Action)actions.get( ConstValue.CoreViewZoomout));
		
		add(viewMenu);

		//窗口菜单		
		windowMenu = new WindowMenu(this.parent,this);
		add(windowMenu);

		/*帮助菜单
		helpMenu = new JMenu();
		topicMenuItem = helpMenu.add((Action)actions.get("help.topic"));
		aboutMenuItem = helpMenu.add((Action)actions.get("help.about"));
		add(helpMenu);*/
		helpMenu = new JMenu();
		topicMenuItem = createMenuItem(helpMenu,ConstValue.CoreHelpTopic,(Action)actions.get(ConstValue.CoreHelpTopic));
		aboutMenuItem = createMenuItem(helpMenu,ConstValue.CoreHelpAbout,(Action)actions.get(ConstValue.CoreHelpAbout));
		add(helpMenu);

		//internal frame 按钮
		buttonPanel = new InternalFramePanel();
		minimizeButton = new InternalFrameButton();
		minimizeButton =
			new InternalFrameButton(
				(Action)actions.get("internalFrame.minimize"));
		minimizeButton.setIcon(iconManager.getIcon("minimize.gif"));
		minimizeButton.setToolTipText(
			ResourceUtil.getRS("internalFrame.minimize"));

		revertButton = new InternalFrameButton();
		revertButton =
			new InternalFrameButton(
				(Action)actions.get("internalFrame.revert"));
		revertButton.setIcon(iconManager.getIcon("revert.gif"));
		revertButton.setToolTipText(ResourceUtil.getRS("internalFrame.revert"));

		closeButton = new InternalFrameButton();
		closeButton =
			new InternalFrameButton((Action)actions.get("internalFrame.close"));
		closeButton.setIcon(iconManager.getIcon("close.gif"));
		closeButton.setToolTipText(ResourceUtil.getRS("internalFrame.close"));

		setButtonVisible(false);

		buttonPanel.add(minimizeButton);
		buttonPanel.add(revertButton);
		buttonPanel.add(closeButton);

		add(buttonPanel);

		loadResources();
	}
	
	public AbstractButton getByName(String menuName){
		return (AbstractButton)menuMap.get(menuName);
	}

	public HashMap getMenuMap(){
		return menuMap;
	}

	private JMenuItem createMenuItem(JMenu pmenu, String menuName,Action act){
		JMenuItem menu=pmenu.add(act);
		menuMap.put(menuName,menu);
		return menu;
	}

	public void setDefaultAction( String name)
	{
		JMenuItem menu =( JMenuItem)menuMap.get( name);
		if( menu == null) return;
		Map actions = parent.getActions();
		if( !actions.containsKey( name))
			return;
		menu.setAction( (Action)actions.get( name));
	}
	
	/**
	 * when the inframe is maxized ,the three button:max ,revert,mini will be visible
	 * on the top right corner of the panel
	 * @param b
	 */
	public void setButtonVisible(boolean b)
	{
		minimizeButton.setVisible(b);
		revertButton.setVisible(b);
		closeButton.setVisible(b);
	}
	
	/**
	 * when all the inframes are closed  ,the current open window checkItem of windowmenu is moved away
	 *
	 */
	public void removeCheckMenus()
	{
		JInternalFrame[] allFrames = parent.getDesktopPanel().getAllFrames();
		int count = windowMenu.getItemCount();
		for (int i = count - 1; i >= 0; i--)
		{
			Component component = windowMenu.getItem(i);
			if (component instanceof JCheckBoxMenuItem)
			{
				windowMenu.remove(component);
			}
		}
	}
	
	/**
	 * 在此处设置 内窗体最大化时右上角的按钮是否可见
	 */
	public void addCheckItem(OBEInternalFrame inFrame)
	{
		windowMenu.addCheckItem(inFrame);
	}
	
	public WindowMenu getWindowMenu()
	{
		return windowMenu;
	}

	public OBEDesigner getDesignerParent()
	{
		return parent;
	}
	
	public JCheckBoxMenuItem getTabBarItem()
	{
		return tabBarItem;
	}
	
	public JCheckBoxMenuItem getToolBarItem()
	{
		return toolBarItem;
	}
	
	public JCheckBoxMenuItem getStatusBarItem()
	{
		return statusBarItem;
	}

}
