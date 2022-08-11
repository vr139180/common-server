package com.cms.designer.workflow.editor;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.File;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: ProjectEditor.java,v 1.3 2004/08/06 09:00:00 jefferyd Exp $
 * $Id: ProjectEditor.java,v 1.3 2004/08/06 09:00:00 jefferyd Exp $
 */
public class ProjectEditor extends JPanel
{
	private OBEDesigner parent =null;

	private JTextField nameField;
	
	private ProjectData projectData =null;

	public ProjectEditor(OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}

	/**
	 * @return
	 */
	public ProjectData getProjectData()
	{
		return projectData;
	}

	/**
	 * 初始化对话框
	 */
	private void init()
	{
		GridBagLayout gbl = new GridBagLayout();
		GridBagConstraints gbc = new GridBagConstraints();
		setLayout(gbl);

		gbc.insets = new Insets(1, 1, 1, 1);
		gbc.anchor = GridBagConstraints.WEST;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		JLabel nameLabel = new JLabel(ResourceUtil.getRS("newProject.name"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints(nameLabel, gbc);
		add(nameLabel);

		nameField = new JTextField("untitled");
		nameField.setPreferredSize(new Dimension(220, 22));
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints(nameField, gbc);
		add(nameField);
	}
	
	/**
	 * 保存新建的项目
	 * @return
	 */
	public boolean save()
	{
		String name = null;
		String path = null;

		if(nameField==null)
		{
			nameField = new JTextField();
			nameField.setText("default");
		}

		name = nameField.getText();
		path = System.getProperty("ide.home")+File.separator+"workspace"+File.separator;
		
		if(name==null||name.trim().length()<=0)
		{
			JOptionPane.showConfirmDialog(null,"请填写名称.","名称不能为空.",JOptionPane.DEFAULT_OPTION);
			return false;
		}

		ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
		projectData =pm.getOneProjectByName(name);
		
		if(projectData!=null)
		{
		  	JOptionPane.showConfirmDialog(null,"请为该项目另取名称.","项目:"+name+"已存在!",JOptionPane.DEFAULT_OPTION);
		  	return false;
		}
		else
			projectData =new ProjectData( name, path);
		
		return true;
	}

}
