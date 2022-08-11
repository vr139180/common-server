package com.cms.designer.swingui.menu;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JDesktopPane;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEDesktopPane;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.swingui.OBEMenuBar;
import com.cms.designer.util.ConstValue;
import com.cms.designer.workflow.OBEGraphWorkflow;

public class WindowMenu extends JMenu
{
	public IconManager iconManager =com.cms.designer.util.ResourceUtil.getIconManager();

	private OBEDesigner parent =null;
	private OBEMenuBar menubar=null;
	
	//窗口菜单项
	private JMenu windowMenu;
	private JMenuItem cascadeItem,
		horizontalItem,	verticalItem, closeItem, currentItem;

	//总数
	private int count = 0, frameCount = 0;
	
	//the number of inframe to be displayed
	private OBEInternalFrame inFrame;
	private JDesktopPane desktopPane;

	//frame_x,_y 是graphpanel(splitpane)的左上角的坐标,deskWidth是tabsplit
	// 的宽度,framewidth是inframe的宽度
	private int x = 50,	y = 50,	deskWidth = 0,	deskHeight = 0,	frameWidth = 0,
		frameHeight = 0, frame_x = 0, frame_y = 0;

	public WindowMenu(OBEDesigner parent,OBEMenuBar menubar)
	{
		this.parent = parent;
		this.menubar =menubar;
		init();
	}

	/**
	 * 初始化窗口管理菜单
	 */
	public void init()
	{
		windowMenu = new JMenu();

		desktopPane = new JDesktopPane();
		desktopPane.setOpaque(true);
		desktopPane.setBackground(Color.LIGHT_GRAY);

		//层叠菜单
		cascadeItem = new JMenuItem();
		cascadeItem.setMnemonic('c');
		cascadeItem.setIcon(iconManager.getIcon("cascade.gif"));
		cascadeItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				count = getDesignerParent().vectorCount();
				x = 0;
				y = 0;
				OBEDesktopPane.isMaxState = false;
				if (count > 0)
				{
					desktopPane = getDesignerParent().getDesktopPanel();
					int divider = getDesignerParent().splitLeftPanelWidth();
					getDesignerParent().tabSplit.setDividerLocation( divider + 1);
					JInternalFrame[] allframes =desktopPane.getAllFrames();
					for (int i = count - 1; i >= 0; i--)
					{
						inFrame =(OBEInternalFrame)allframes[i];
						/*
						if (inFrame.getScrollPane().getViewport().getView() == null)
						{
							inFrame.getScrollPane().getViewport().setView(
								inFrame.getGraph());
						}
						*/
						if (OBEDesktopPane.isMaxState == true)
						{
							try
							{
								parent.getOBEMenuBar().setButtonVisible(false);
							}
							catch (Exception ex)
							{
								ex.printStackTrace();
							}
						}
						inFrame.setSize(400, 300);
						inFrame.setLocation(x, y);
						x = x + 20;
						y = y + 20; //----改动时要添加:x, y 要归零
					}

					getDesignerParent().getDesktopPanel().getDesktopManager().activateFrame( inFrame);
				}
			}

		});
		add(cascadeItem);
		menubar.getMenuMap().put(ConstValue.CoreWinCascade,cascadeItem);
		
		///---------------horizontal
		horizontalItem = new JMenuItem();
		horizontalItem.setMnemonic('c');
		horizontalItem.setIcon(iconManager.getIcon("dstrb_hc.gif"));
		horizontalItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				count =	getDesignerParent().getDesktopPanel().getUnIconedWindowCount();
				deskWidth = getDesignerParent().splitLeftPanelWidth();
				deskHeight = getDesignerParent().splitLeftPanelHeight();
				OBEDesktopPane.isMaxState = false;

				if (count > 0)
				{
					frameWidth = (int)deskWidth / count;
					frame_x = 0;
					frame_y = 0;
					desktopPane = getDesignerParent().getDesktopPanel();
					int divider = getDesignerParent().splitLeftPanelWidth();
					getDesignerParent().tabSplit.setDividerLocation(divider + 1);
					JInternalFrame[] allframes =desktopPane.getAllFrames();
					for (int i = 0; i < count; i++)
					{
						inFrame =(OBEInternalFrame)allframes[i];
						/*
						if (inFrame.getScrollPane().getViewport().getView() == null)
						{
							inFrame.getScrollPane().getViewport().setView(inFrame.getGraph());
						}*/
						
						if (OBEDesktopPane.isMaxState == true)
						{
							parent.getOBEMenuBar().setButtonVisible(false);
						}
						inFrame.setSize(frameWidth, deskHeight);
						inFrame.setLocation(frame_x, frame_y);
						frame_x = frame_x + frameWidth;
					}
					//getDesignerParent().getDesktopPanel().getDesktopManager().activateFrame(inFrame);
				}
			}
		});
		add(horizontalItem);
		menubar.getMenuMap().put(ConstValue.CoreWinHorizontal,horizontalItem);
		////////////////   Layout of vertical
		verticalItem = new JMenuItem();
		verticalItem.setIcon(iconManager.getIcon("dstrb_vc.gif"));
		verticalItem.setMnemonic('c');
		verticalItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				count =
					getDesignerParent().getDesktopPanel().getUnIconedWindowCount();
				deskWidth = getDesignerParent().splitLeftPanelWidth();
				deskHeight = getDesignerParent().splitLeftPanelHeight();
				OBEDesktopPane.isMaxState = false;
				if (count > 0)
				{
					frameHeight = (int)deskHeight / count;
					frame_x = 0;
					frame_y = 0;
					desktopPane = getDesignerParent().getDesktopPanel();
					int divider = getDesignerParent().splitLeftPanelWidth();
					getDesignerParent().tabSplit.setDividerLocation(divider + 1);
					JInternalFrame[] allframes =desktopPane.getAllFrames();
					for (int i = 0; i < count; i++)
					{
						inFrame =(OBEInternalFrame)allframes[i];
						/*
						if (inFrame.getScrollPane().getViewport().getView()	== null)
						{
							inFrame.getScrollPane().getViewport().setView(inFrame.getGraph());
						}
						*/
						if (OBEDesktopPane.isMaxState == true)
						{
							parent.getOBEMenuBar().setButtonVisible(false);
						}
						inFrame.setSize(deskWidth, frameHeight);
						inFrame.setLocation(frame_x, frame_y);
						frame_y = frame_y + frameHeight;
					}
					//getDesignerParent().getDesktopPanel().getDesktopManager().activateFrame(inFrame);
				}
			}
		});
		add(verticalItem);
		menubar.getMenuMap().put(ConstValue.CoreWinVertical,verticalItem);
		/////--------close all frame
		closeItem = new JMenuItem();
		closeItem.setIcon(iconManager.getIcon("tm.gif"));
		closeItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				count = getDesignerParent().vectorCount();
				OBEDesktopPane.INCREASE = 0;
				if (count > 0)
				{
					desktopPane = getDesignerParent().getDesktopPanel();
					int divider = getDesignerParent().splitLeftPanelWidth();
					parent.getOBEMenuBar().setButtonVisible(false);
					JInternalFrame[] allframes =desktopPane.getAllFrames();
					for (int i = 0; i < count; i++)
					{
						inFrame =(OBEInternalFrame)allframes[0];
						//vector 当用remove时,节点数自动调整
						/*
						if (inFrame.getScrollPane().getViewport().getView()
							== null)
						{
							inFrame.getScrollPane().getViewport().setView(
								inFrame.getGraph());
						}
						*/
						if (inFrame.isClosed() == false)
						{
							try
							{
								inFrame.setClosed(true);
							}
							catch (java.beans.PropertyVetoException ex)
							{
							}
						}
					}
					getDesignerParent().tabSplit.setDividerLocation( divider + 1);
					getDesignerParent().setFrameTitle( null);
				}
			}
		});
		add(closeItem);
		menubar.getMenuMap().put(ConstValue.CoreWinClose,closeItem);
		
		JSeparator separator = new JSeparator();
		//add(separator);

		cascadeItem.setText(ResourceUtil.getRS("windowMenu.cascade"));
		horizontalItem.setText(ResourceUtil.getRS("windowMenu.horizontal"));
		verticalItem.setText(ResourceUtil.getRS("windowMenu.vertical"));
		closeItem.setText(ResourceUtil.getRS("windowMenu.close"));
		
		cascadeItem.setEnabled(false);
		horizontalItem.setEnabled(false);
		verticalItem.setEnabled(false);
		closeItem.setEnabled(false);
	}
	
	public CheckMenuItem getCheckMenuItem(String name)
	{
		CheckMenuItem checkItem = null;
		int itemCount = this.getItemCount();
		for (int i = itemCount - 1; i >= 0; i--)
		{
			JMenuItem item = (JMenuItem)this.getItem(i);
			if (item == null||item.getName()==null)
			{
				break;
			}
			if (item.getName().equals(name))
			{
				checkItem = (CheckMenuItem)item;
				break;
			}
		}
		return checkItem;
	}
	
	public void addCheckItem(OBEInternalFrame inFrame)
	{
		CheckMenuItem checkItem = new CheckMenuItem(inFrame);
		add(checkItem);
		parent.getCheckMenuGroup().add(checkItem);
		parent.getCheckMenuGroup().setSelected(checkItem.getModel(), true);
	}
	
	private OBEDesigner getDesignerParent()
	{
		return parent;
	}

	private void graphCount()
	{
		count = getDesignerParent().vectorCount();
	}

	private JDesktopPane getDeskPane()
	{
		return desktopPane;
	}
	
	public void removeGraphMenuItem( OBEGraphWorkflow graph)
	{
		JMenuItem item = this.getCheckMenuItem(graph.getName());
		this.remove(item);
	}
}