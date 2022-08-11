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
 * ��Ŀ�������ĵ���ʽ�˵�����
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: OBETreePopupMenu.java,v 1.2 2004/08/12 02:47:37 jefferyd Exp $
 */
public class OBETreePopupMenu extends JPopupMenu
{
	private OBEDesigner parent =null;
	//�ڵ���Ϣ
	private OBETreeNode node =null;
	private String type;
	private String key;

	//�����Ƿ��в˵������Ƿ���ʾ�˵�
	private boolean showit =false;

	private JMenu newMenu;
	//��Ŀ�˵�
	private JMenuItem newProjectItem;
	
	//����
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
	 * �������ͳ�ʼ������ʽ�˵�
	 */
	private void init()
	{
		Map actions = parent.getActions();
		showit =true;

		if (type.equals(OBETreeNode.ROOT))
		{ 
			//��ʼ�����˵�
			initRootPop(actions);
		}
		else if (type.equals(OBETreeNode.PROJECT))
		{
			//��ʼ�����̲˵�
			initProjectPop(actions);
		}
		else if( type.equals( OBETreeNode.MODULE))
		{
			//����ģ��˵�
			dealModuleMenu();
		}

		loadResources();
	}
	
	/**
	 * ����˵�
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
	 * ����˵���Դ
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
	 * ��ʼ�����ڵ�
	 * @param actions
	 */
	private void initRootPop(Map actions)
	{
		newMenu = new JMenu();
		
		newProjectItem = newMenu.add( (Action)actions.get( ConstValue.ProjectNew));
		add(newMenu);
		
		addSeparator();
		//������Ŀ
		importItem = add((Action)actions.get( ConstValue.ProjectImport));
	}

	/**
	 * ��ʼ�����̲˵�
	 * @param actions
	 */
	private void initProjectPop( Map actions)
	{
		newProcessItem =add((Action)actions.get( WorkFlowMenuManager.ACT_NEWPROCESS));

		//��������
		exportItem = add((Action)actions.get( ConstValue.ProjectExport));
		addSeparator();
		deleteItem = add((Action)actions.get( ConstValue.ProjectDelete));
	}

	public boolean isShowit()
	{
		return showit;
	}
}
