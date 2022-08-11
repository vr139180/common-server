package com.cms.designer.workflow.editor;

import java.awt.Dimension;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.cms.core.BasicElement;
import com.cms.core.util.ResourceUtil;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: EntityEditor.java,v 1.2 2004/08/26 05:00:54 jefferyd Exp $
 */
public class EntityEditor extends JPanel
{
	private BasicElement element;

	private JTextField nameField;
	
	public EntityEditor()
	{
		init();
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

	public String getId()
	{
		return element.getId();
	}

	public boolean save()
	{
		if (element != null)
			element.setName(nameField.getText());
		
		return true;
	}

	public void revert()
	{
		if (element != null)
		{
			nameField.setText( element.getName());
		}
	}

	private void init()
	{
		this.setLayout( null);
		this.setPreferredSize( new Dimension( 280, 240));

		JLabel nameLabel = new JLabel(ResourceUtil.getRS("detail.name"));
		add(nameLabel);
		nameLabel.setBounds( 5, 5, 30, 25);

		nameField = new JTextField();
		add(nameField);
		nameField.setBounds( 35, 5, 220, 25);

	}
}
