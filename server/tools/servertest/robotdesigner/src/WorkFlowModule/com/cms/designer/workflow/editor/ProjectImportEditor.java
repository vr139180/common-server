package com.cms.designer.workflow.editor;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.util.FileUtil;

/**
 * @author zagio
 * 
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments $Id:
 * ProjectImportEditor.java,v 1.1.2.2 2004/06/24 04:10:52 gaj Exp $
 */
public class ProjectImportEditor extends JPanel
{
	private OBEDesigner parent = null;

	private JTextField nameField;

	private JTextField pathField;

	private JButton browseButton;

	public ProjectImportEditor( OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}

	/**
	 * 初始化对话框
	 */
	private void init()
	{
		GridBagLayout gbl = new GridBagLayout();
		GridBagConstraints gbc = new GridBagConstraints();
		setLayout( gbl);

		gbc.insets = new Insets( 1, 1, 1, 1);
		gbc.anchor = GridBagConstraints.WEST;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		JLabel nameLabel = new JLabel( ResourceUtil.getRS( "newProject.name"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints( nameLabel, gbc);
		add( nameLabel);

		nameField = new JTextField();
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints( nameField, gbc);
		add( nameField);

		JLabel pathLabel = new JLabel( ResourceUtil.getRS( "dialog.path"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints( pathLabel, gbc);
		add( pathLabel);

		pathField = new JTextField( System.getProperty( "ide.home"));
		pathField.setPreferredSize( new Dimension( 220, 22));
		pathField.setEditable( false);
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.RELATIVE;
		gbl.setConstraints( pathField, gbc);
		add( pathField);

		browseButton = new JButton( ResourceUtil.getRS( "dialog.browse"));
		gbc.weightx = 0;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints( browseButton, gbc);
		browseButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent e)
			{
				browseFile();
			}
		});
		add( browseButton);
	}

	public boolean canSave()
	{
		String rename =nameField.getText();
		rename =rename.trim();
		if( rename.length() <= 0)
		{
			JOptionPane.showConfirmDialog( null, "项目名称不能为空.", "错误",
					JOptionPane.DEFAULT_OPTION);
			return false;
		}

		File srcFile = new File( this.pathField.getText());
		File ff =new File( srcFile, "project.wpr");
		if( !ff.exists())
		{
			JOptionPane.showConfirmDialog( null, "选择的目录不包含项目信息", "错误",
					JOptionPane.DEFAULT_OPTION);
			return false;
		}

		ProjectManager pm = OBEModuleManager.getInstance().getProjectManager();
		ProjectData projectData = pm.getOneProjectByName( nameField.getText());
		if( projectData != null)
		{
			JOptionPane.showConfirmDialog( null, "请为该项目另取名称.",
					"项目:" + nameField.getText() + "已存在!", JOptionPane.DEFAULT_OPTION);
			return false;
		}

		File destFile = new File( System.getProperty( "ide.home") +File.separator+"workspace"
			+File.separator+ nameField.getText());
		if( destFile.exists())
		{
			JOptionPane.showConfirmDialog( null, "请另选项目名称.", "项目名称已经存在!",
					JOptionPane.DEFAULT_OPTION);
			return false;
		}

		return true;
	}
	/**
	 * export this project into another dir
	 * 
	 * @return
	 */
	public boolean save()
	{
		File srcFile = new File( this.pathField.getText());

		ProjectManager pm = OBEModuleManager.getInstance().getProjectManager();
		File destFile = new File( System.getProperty( "ide.home") +File.separator+"workspace"
			+File.separator+ nameField.getText());

		FileUtil.copyDir( srcFile, destFile);

		//load this project into memory
		ProjectData pd = ProjectData.loadProject( nameField.getText(), System
				.getProperty( "ide.home")+File.separator+"workspace");

		if( pd == null)	return false;
		
		pm.newProject( pd);
		pd.loadProject();

		//refresh the project tree
		parent.getWorkSpaceManager().refrushWorkspaceTree();
		//refresh config file workspace.properties
		parent.getWorkSpaceManager().saveProjects();
		
		return true;
	}

	private void browseFile()
	{
		JFileChooser chooser = new JFileChooser( OBEDesigner.getWorkingDirectory());
		chooser.setFileSelectionMode( JFileChooser.DIRECTORIES_ONLY);
		chooser.setCurrentDirectory( new File( System.getProperty( "ide.home")));
		if( chooser.showDialog( this, ResourceUtil.getRS( "file.select")) == JFileChooser.APPROVE_OPTION)
		{
			try
			{
				File selectedFile = chooser.getSelectedFile();
				this.pathField.setText( selectedFile.toString());
				this.nameField.setText( selectedFile.getName());
			}
			catch( Throwable t)
			{
				t.printStackTrace();
			}
		}
	}

}
