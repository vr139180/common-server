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
 * ����ģ��������
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: OBEModuleStandard.java,v 1.4 2004/08/23 02:36:10 york Exp $
 */
public abstract class OBEModuleStandard
{
	//ͼƬ��Դ
	public static IconManager iconManager = com.cms.designer.util.ResourceUtil.getIconManager();

	private OBEDesigner parent = null;
	//ģ������
	private OBEModule descriptions =null;
	
	private final Long lockOBJ =new Long( 0);
	//�����ص�ģ������
	private final HashMap moduleDatas =new HashMap();
	private long uuid =0;
	
	//��Ź�������ť
	private OBEComponentBar toolbar =null;
	//���ϵͳ�˵�����ʵ��
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
		//��ʼ��������
		toolbar =new OBEComponentBar( parent);
	}

	//��ȡģ��˵�
	public abstract JMenu getModuleMenu();
	//���ݱ�ʾ��Ϣ��ȡ����ʽ�˵�
	public abstract boolean getPopupMenu( JPopupMenu root, String moduleExtAtt);
	//��ȡ�յ�ģ����������
	public abstract OBEModuleData newModuleData( String projectID, String moduleID);
	//��ȡģ��edit����
	public abstract OBEInternalFrame newModuleFrame( String key);
	//�ж��Ƿ�ʹ��������
	public abstract boolean canOpenView( String moduleExtAtt);
	//����ָ��·����ģ����Ϣ
	public abstract OBEModuleData loadModuleData( String path, String projectID);

	//��ȡ��������Ϣ
	public final OBEComponentBar getModuleToolbar()
	{
		return toolbar.getToolbar();
	}

	/**
	 * ����һ��ģ������
	 * @param md
	 * @return
	 */
	public final synchronized boolean addModuleData( OBEModuleData md)
	{
		boolean ret =true;
		//����uuid
		synchronized( lockOBJ)
		{
			++uuid;
			md.setUuid( Long.toString( uuid));
			moduleDatas.put( md.getUuid(), md);
		}
		
		return ret;
	}
	
	/**
	 * ɾ��ģ������
	 * @param uuid
	 */
	public final synchronized void removeModuleData( String uuid)
	{
		moduleDatas.remove( uuid);
	}
	
	/**
	 * ����uuid��ȡ����
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

	//��ȡȱʡ�Ľڵ�ͼ��
	public ImageIcon getDefaultNodeImage()
	{
		return null;
	}
	
	//��ȡ�ƶ����ƵĽڵ�ͼ��
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
	 * ע���õ��İ�ť
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
		
		//���ӵ�������
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
			
		//���ӵ�������
		toolbar.addButton( name, button);
			
		return button;
	}
	
	/**
	 * ע��ϵͳ�Ķ���
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
	 * ��ȡϵͳ����
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
