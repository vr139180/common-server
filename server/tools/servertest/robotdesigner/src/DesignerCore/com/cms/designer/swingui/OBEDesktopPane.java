package com.cms.designer.swingui;

import java.awt.Dimension;

import javax.swing.JDesktopPane;
import javax.swing.JInternalFrame;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.anthonyeden.lib.util.IconManager;
import com.cms.designer.util.ResourceUtil;

/**
 * @author Administrator
 * 管理childframe
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEDesktopPane extends JDesktopPane
{
	private static final Log log = LogFactory.getLog(OBEDesktopPane.class);
	private static IconManager iconManager =ResourceUtil.getIconManager();

	//缺省参数inernalframe
	public static final int FRAME0_X = 0;
	public static final int FRAME0_Y = 0;
	public static final int FRAME0_WIDTH = 320;
	public static final int FRAME0_HEIGHT = 230;
	public static int INCREASE = 0;
	public static Dimension dim = new Dimension(100, 100);
	public static boolean isMaxState = false;

	//层数
	public Integer PALETTE_LAYER = new Integer(3);
	private OBEDesigner parent;

	public OBEDesktopPane(OBEDesigner parent)
	{
		this.parent = parent;
	}

	public OBEInternalFrame getCurrentInternalFrame()
	{
		return (OBEInternalFrame)this.getSelectedFrame();
	}
	
	/**
	 * 判断窗口是否打开
	 * @param key
	 * @return
	 */
	public boolean isOpen( String key)
	{
		boolean ret =false;
		JInternalFrame[] frames =this.getAllFrames();
		for( int i =0; i<frames.length; ++i)
		{
			OBEInternalFrame frame = (OBEInternalFrame)frames[i];
			if( key.equals( frame.getName()))
			{
				ret =true;
				break;
			}
		}
		
		return ret;
	}
	
	public void addInternalFrame( OBEInternalFrame frame, boolean visible)
	{
		add( frame, PALETTE_LAYER, 0);

		frame.setSelected();
		INCREASE += 20;
		setCurrentInternalFrame( frame);
		
		if (isMaxState)
			getCurrentInternalFrame().setMaximum( true);

		frame.setVisible(visible);
		
	}

	public void setCurrentInternalFrame(String name)
	{
		OBEInternalFrame internalFrame = getInternalFrame(name);
		if (internalFrame != null)
			setCurrentInternalFrame(internalFrame);
	}
	
	public void setCurrentInternalFrame( OBEInternalFrame internalFrame)
	{
		int length = this.getAllFrames().length;
		for (int i = 0; i < length; i++)
		{
			OBEInternalFrame frame = (OBEInternalFrame)getAllFrames()[i];
			if (frame != internalFrame)
			{
				try
				{
					frame.setSelected(false);
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		}
		this.setSelectedFrame(internalFrame);
	}

	public OBEInternalFrame getLastInternalFrame()
	{
		return (OBEInternalFrame)getAllFrames()[getAllFrames().length - 1];
	}
	
	public OBEInternalFrame getNextInternalFrame(OBEInternalFrame internalFrame)
	{
		OBEInternalFrame nextInternalFrame = null;
		boolean find = false;
		for (int i = 0; i < getAllFrames().length; i++)
		{
			OBEInternalFrame findFrame = (OBEInternalFrame)getAllFrames()[i];
			if (find)
			{
				nextInternalFrame = findFrame;
				find = false;
				break;
			}
			if (findFrame.getName().equals(internalFrame.getName()))
			{
				find = true;
			}
		}
		if (nextInternalFrame == null && getAllFrames().length > 1)
		{
			nextInternalFrame = (OBEInternalFrame)getAllFrames()[0];
		}
		return nextInternalFrame;
	}
	
	public void setNextFrameSelected()
	{
		this.getDesktopManager().activateFrame(getAllFrames()[0]);
	}
	
	public void setFrameSelected()
	{
		if( hasSelectedFrame() || hasNoFrame())
			return;
		setNextFrameSelected();
	}
	
	private boolean hasNoFrame()
	{
		boolean noFrame = false;
		if (this.getAllFrames().length == 0)
			noFrame = true;
		return noFrame;
	}
	
	public boolean hasSelectedFrame()
	{
		boolean has = false;
		for (int i = 0; i < this.getAllFrames().length; i++)
		{
			OBEInternalFrame internalFrame =
				(OBEInternalFrame)getAllFrames()[i];
			if (internalFrame.isSelected())
			{
				has = true;
				break;
			}
		}
		
		return has;
	}
	
	/**
	 * 显示制定名称的frame
	 * @param name
	 */
	public void showFrame(String name)
	{
		OBEInternalFrame internalFrame = getInternalFrame(name);
		if (internalFrame == null)
			return;

		parent.setFrameTitle( internalFrame.getTitle());

		if (internalFrame.isClosed())
		{
			internalFrame.show();
			add(internalFrame, PALETTE_LAYER, 0);
			internalFrame.setSelected();
			parent.getOBEMenuBar().getWindowMenu().addCheckItem(internalFrame);
			try
			{
				internalFrame.setClosed(false);
				internalFrame.setIcon(false);
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
			}
		}
		setCurrentInternalFrame(name);

		getCurrentInternalFrame().setSelected();
		if (isMaxState())
			getCurrentInternalFrame().setMaximum( true);
	}
	
	public void showMaxFrame()
	{
	}
	
	public void showNormalFrame()
	{
		int divider = parent.splitLeftPanelWidth();
		parent.tabSplit.setDividerLocation(divider + 1);
		parent.getOBEMenuBar().setButtonVisible(false);
		
		OBEInternalFrame internalFrame = getCurrentInternalFrame();
//		internalFrame.getScrollPane().getViewport().setView(agentGraph);
//		internalFrame.setGraph(agentGraph);

		isMaxState = false;
		internalFrame.setSelected();
		parent.setFrameTitle( internalFrame.getTitle());
	}

	public boolean isMaxState()
	{
		OBEInternalFrame internalFrame = null;
		boolean maxState = false;
		for (int i = 0; i < getAllFrames().length; i++)
		{
			internalFrame = (OBEInternalFrame)getAllFrames()[i];
			if (internalFrame.isMaximum())
			{
				maxState = true;
				break;
			}
		}
		return maxState;
	}

	public OBEInternalFrame getInternalFrame(String name)
	{
		OBEInternalFrame internalFrame = null;
		boolean has = false;
		JInternalFrame[] allFrames = this.getAllFrames();
		for (int i = 0; i < allFrames.length; i++)
		{
			internalFrame = (OBEInternalFrame)allFrames[i];
			if (internalFrame.getName().equals(name))
			{
				has = true;
				break;
			}
		}
		if (!has)
			internalFrame = null;
		return internalFrame;
	}

	public int getUnIconedWindowCount()
	{
		int count = 0;
		for (int i = 0; i < this.getAllFrames().length; i++)
		{
			OBEInternalFrame internalFrame =
				(OBEInternalFrame)getAllFrames()[i];
			if (internalFrame.isIcon() == false)
			{
				count++;
			}
		}
		return count;
	}
}
