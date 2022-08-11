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
	
	//����
	private JTextField nameField;

	//������Ŀ��Ϣ
	private String elementKey;
	
	public ActivityNormalInformationPanel( String key, Activity activity)
	{
		this.elementKey =key;
		this.activity =activity;
		
		//ֻ�п�ʼ�ڵ㣬�����ڵ���˹��ڵ�����й鵵����
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

	//����
	public boolean save()
	{
		if(nameField.getText()==null||nameField.getText().trim().length()<=0){
			JOptionPane.showConfirmDialog(null,"����д����.","���Ʋ���Ϊ��.",JOptionPane.DEFAULT_OPTION);
			return false;
		}

		if( element != null)
		{
			element.setName(getName());
		}
		
		return true;
	}

	//��ʼ����������
	public void revert()
	{
		if( element != null)
		{
			nameField.setText(element.getName());
		}
	}

	//��ʼ������
	private void jbInit() throws Exception
	{
		setBackground(SystemColor.control);
		setPreferredSize( new Dimension( 250, 200));
		setLayout(null);

		JLabel nameLabel = new JLabel();
		nameLabel.setText( ResourceUtil.getRS("activity.name"));
		nameLabel.setBounds( new Rectangle( 5, 3, 45, 24));
		this.add(nameLabel, null);
		
		nameField = new JTextField();
		nameField.setBounds( new Rectangle( 53, 3, 180, 24));
		this.add(nameField, null);
		//�༭������Ϣʱ�����޸�����
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