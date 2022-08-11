package com.cms.designer.swingui;

import java.beans.PropertyVetoException;

import javax.swing.ImageIcon;
import javax.swing.JInternalFrame;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;

import com.anthonyeden.lib.util.IconManager;
import com.cms.designer.beacon.DefaultFrameBeacon;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.swingui.menu.CheckMenuItem;
import com.cms.designer.util.ResourceUtil;

/**
 * @author Administrator
 * 
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEInternalFrame.java,v 1.2 2004/06/30 09:19:07 jefferyd Exp $
 * $Id: OBEInternalFrame.java,v 1.2 2004/06/30 09:19:07 jefferyd Exp $
 */
public class OBEInternalFrame extends JInternalFrame
{
	//ͼ�����
	public static IconManager iconManager =ResourceUtil.getIconManager();

	private OBEDesigner parent =null;
	private OBEModuleStandard module =null;
	
	private String title;
	
	private OBEMenuBar menuBar;

	public int frameX = 0;
	public int frameY = 0;
	public int frameWidth = 400;
	public int frameHeight = 300;
	
	private boolean dirty=false;
	
	public OBEInternalFrame( OBEDesigner parent, OBEModuleStandard module, String title)
	{
		super( title, true, true, true);
		this.parent = parent;
		this.module =module;
		this.menuBar = parent.getOBEMenuBar();
		this.title = title;
		this.setName( title);
		this.setDefaultCloseOperation( DISPOSE_ON_CLOSE);
		if(module.getDefaultBeacon()==null)
			module.setDefaultBeacon(new DefaultFrameBeacon(parent));
			
		init();
	}

	/**
	 * ��ʼ������
	 */
	private void init()
	{
		this.setIconifiable(true);
		//����frame��ͼ��
		this.setFrameIcon( (ImageIcon)iconManager.getIcon("workflow.gif"));

		frameX += OBEDesktopPane.INCREASE;
		frameY += OBEDesktopPane.INCREASE;
		setBounds(frameX, frameY, frameWidth, frameHeight);
//		menuBar.addCheckItem(this);

		//���Ӽ�����
		this.addInternalFrameListener( new InternalFrameAdapter()
		{
			public void internalFrameClosed(InternalFrameEvent e)
			{	
				//�޸Ĳ˵�
				parent.getDesktopPanel().setFrameSelected();
				CheckMenuItem checkItem =
					parent.getOBEMenuBar().getWindowMenu().getCheckMenuItem(title);
				if (checkItem != null)
					checkItem.close();
				
				if (parent.getDesktopPanel().getAllFrames().length == 0)
				{
					OBEDesktopPane.INCREASE = 0;
					getDesignerParent().setFrameTitle( null);
					
					if(getModule().getDefaultBeacon()!=null)
						getModule().getDefaultBeacon().sendMessage();
				}
				
			}
			public void internalFrameIconified(InternalFrameEvent e)
			{
				//�Զ�ѡ����һ������
				parent.getDesktopPanel().setFrameSelected();
			}
			public void internalFrameActivated(InternalFrameEvent e)
			{
				//�������ʱ�� �޸Ŀ�ܲ˵��͹�����
				parent.setFrameTitle( getTitle());
				if (title != null)
				{
					CheckMenuItem checkItem =
						parent.getOBEMenuBar().getWindowMenu().getCheckMenuItem( title);
					if (checkItem != null)
						checkItem.setSelected(true);
				}
				
				//�޸���ͼ����صĲ˵���������
				changeView( e.getInternalFrame());
				
				if(getModule().getBeacon()!=null)
					getModule().getBeacon().sendMessage();
			}
			public void internalFrameDeactivated(InternalFrameEvent e)
			{
				//�޸Ŀ�ܲ˵��͹�����
				viewDeactivated( e.getInternalFrame());
			}
		});
	}

	/**
	 * ��ͼ�����任
	 * @param frame
	 */
	public void changeView( JInternalFrame frame)
	{
		parent.changeView( frame.getName());
	}
	
	/**
	 * ��ͼʧȥ����
	 * @param frame
	 */
	public final void viewDeactivated( JInternalFrame frame)
	{	
		//close toolbar
		if( parent.getDesktopPanel().getAllFrames().length <= 0)
			return;
		parent.viewDeactivated( frame.getName());
	}

	public OBEDesigner getDesignerParent()
	{
		return parent;
	}

	public void fireInternalFrameEvent(int id)
	{
		super.fireInternalFrameEvent(id);
	}

	public void setMaximum(boolean b)
	{
		try
		{
			super.setMaximum( b);
		}
		catch( PropertyVetoException pv){
			pv.printStackTrace();
		}
		
		//�ڴ�������,�����оͻ���С��
		parent.getDesktopPanel().setCurrentInternalFrame(this.getName());
		if (b == true)
		{
		}
		else
		{
			parent.getDesktopPanel().showNormalFrame();
		}
	}

	public void setSelected()
	{
		try
		{
			this.setSelected(true);
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}

	public OBEMenuBar getOBEMenuBar()
	{
		return parent.getOBEMenuBar();
	}

	public void revert()
	{
	}

	public void close()
	{}
	
	public String toString()
	{
		return "OBEInternalFrame[ " + this.getName() + " ]";
	}

	/**
	 * @return
	 */
	public String getTitle()
	{	
		if( dirty)
			return title +"*"+com.cms.core.util.ResourceUtil.getRS("window.notsave");
		else
			return title;
	}

	/**
	 * @param title
	 */
	public void setTitle( String title)
	{
		this.title = title;

		super.setTitle( getTitle());
	}

	/**
	 * @return
	 */
	public OBEModuleStandard getModule()
	{
		return module;
	}

	public void setFrameModified( boolean dirty)
	{
		this.dirty=dirty;
		
		this.setClosable( !this.dirty);
		
		this.setTitle( title);
		parent.setFrameTitle( getTitle());
	}

	/**
	 * @return Returns the dirty.
	 */
	public boolean isDirty()
	{
		return dirty;
	}
}