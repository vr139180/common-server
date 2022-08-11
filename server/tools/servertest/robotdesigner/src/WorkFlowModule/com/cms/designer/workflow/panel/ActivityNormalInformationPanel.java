package com.cms.designer.workflow.panel;


import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.SystemColor;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.cms.core.BasicElement;
import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.activity.Activity;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: ActivityNormalInformationPanel.java,v 1.9 2004/09/10 01:28:44 york Exp $
 */
public class ActivityNormalInformationPanel extends JPanel
{
	private BasicElement element;

	private Activity activity;
	
	//名称
	private JTextField nameField;

	//保存项目信息
	private String elementKey;
	
	public ActivityNormalInformationPanel( String key, Activity activity)
	{
		this.elementKey =key;
		this.activity =activity;
		
		//只有开始节点，结束节点和人工节点才能有归档功能
		try
		{
			jbInit();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public ActivityNormalInformationPanel( BasicElement element, String key, Activity activity)
	{
		this( key, activity);
		
		setElement(element);
	}

	public BasicElement getElement()
	{
		return element;
	}

	public void setElement(BasicElement element)
	{
		this.element = element;
		revert();
	}

	public String getName()
	{
		return nameField.getText();
	}

	//保存
	public boolean save()
	{
		if(nameField.getText()==null||nameField.getText().trim().length()<=0){
			JOptionPane.showConfirmDialog(null,"请填写名称.","名称不能为空.",JOptionPane.DEFAULT_OPTION);
			return false;
		}

		if( element != null)
		{
			element.setName(getName());
		}
		
		return true;
	}

	//初始化界面数据
	public void revert()
	{
		if( element != null)
		{
			nameField.setText(element.getName());
		}
	}

	//初始化界面
	private void jbInit() throws Exception
	{
		setBackground(SystemColor.control);
		setPreferredSize( new Dimension( 450, 300));
		setLayout(null);

		JLabel nameLabel = new JLabel();
		nameLabel.setText( ResourceUtil.getRS("activity.name"));
		nameLabel.setBounds( new Rectangle( 5, 3, 60, 24));
		this.add(nameLabel, null);
		
		nameField = new JTextField();
		nameField.setBounds( new Rectangle( 70, 3, 240, 24));
		this.add(nameField, null);
		//编辑流程信息时不能修改名称
		if( activity == null)
			nameField.setEditable( false);
	}
	
	class BeanInfo
	{
		private String name =null;
		private String id;
		
		public BeanInfo( String n, String id)
		{
			this.name =n;
			this.id =id;
		}
		
		public String toString()
		{
			return name;
		}
		
		public String getId()
		{
			return id;
		}
	}
}