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
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.util.OBETreeData;
import com.cms.util.FileUtil;

/**
 * @author zagio
 * 
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments $Id:
 * ProjectExportEditor.java,v 1.1.2.1 2004/06/08 03:08:35 gaj Exp $
 */
public class ProjectExportEditor extends JPanel
{
	private OBEDesigner parent = null;

	private JTextField nameField;

	private JTextField pathField;

	private JButton browseButton;

	private ProjectData projectData = null;

	public ProjectExportEditor( OBEDesigner parent)
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
		setLayout( gbl);

		gbc.insets = new Insets( 1, 1, 1, 1);
		gbc.anchor = GridBagConstraints.WEST;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		JLabel nameLabel = new JLabel( ResourceUtil.getRS( "newProject.name"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints( nameLabel, gbc);
		add( nameLabel);

		OBETreeNode node = parent.getTree().getCurSelectedNode();
		String projectName = ((OBETreeData) node.getUserObject()).getName();
		nameField = new JTextField( projectName);
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

	/**
	 * export this project into another dir
	 * 
	 * @return
	 */
	public boolean save()
	{
		OBETreeNode node = parent.getTree().getCurSelectedNode();
		OBETreeData data = (OBETreeData) node.getUserObject();
		ProjectManager pm = OBEModuleManager.getInstance().getProjectManager();
		ElementKey ek = new ElementKey();
		ek.analyze( data.getKey());
		ProjectData pd = pm.getOneProject( ek.getProjectID());

		File srcFile = new File( pd.getProjectPath());
		File destFile = new File( this.pathField.getText(), nameField.getText());

		int flag = -1;
		if( srcFile.equals( destFile))
		{
			JOptionPane.showConfirmDialog( null, "请另选目标路径.", "目标路径和源路径相同!",
					JOptionPane.DEFAULT_OPTION);
			//projectData=null;
			return false;
		}
		else if( destFile.exists())
		{
			flag = JOptionPane.showConfirmDialog( null, "要将目标路径清空吗?", "目标路径不为空!",
					JOptionPane.YES_OPTION);
			//projectData=null;
			if( flag > 0)
				return false;
			FileUtil.deleteFile( destFile);
		}

		FileUtil.copyDir( srcFile, destFile);

		return true;
	}

	private void browseFile()
	{
		JFileChooser chooser = new JFileChooser( OBEDesigner.getWorkingDirectory());
		chooser.setFileSelectionMode( JFileChooser.DIRECTORIES_ONLY);
		chooser.setCurrentDirectory( new File( System.getProperty( "ide.home")));
		chooser.show();
		if( chooser.showDialog( this, ResourceUtil.getRS( "file.select")) == JFileChooser.APPROVE_OPTION)
		{
			try
			{
				File selectedFile = chooser.getSelectedFile();
				this.pathField.setText( chooser.getSelectedFile().toString());
				chooser.hide();
			}
			catch( Throwable t)
			{
				t.printStackTrace();
			}
		}
	}

}
