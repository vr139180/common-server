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
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.filechooser.FileFilter;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class JarSaveEditor extends JPanel
{
	private OBEDesigner parent =null;

	private JLabel pathLabel;

	private JTextField nameField;
	private JTextField pathField;
	private JButton browseButton;

	public JarSaveEditor( OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}

	public String getDestPath()
	{
		return pathField.getText();
	}
	
	public String getDestName()
	{
		return nameField.getText();
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

		JLabel nameLabel = new JLabel( ResourceUtil.getRS( "dialog.name"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints( nameLabel, gbc);
		add( nameLabel);

		nameField = new JTextField( ResourceUtil.getRS( "default.jarname"));
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints( nameField, gbc);
		add( nameField);

		pathLabel = new JLabel(ResourceUtil.getRS( "dialog.path"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints(pathLabel, gbc);
		add(pathLabel);

		pathField = new JTextField(System.getProperty( "ide.home"));
		pathField.setEditable( false);
		pathField.setPreferredSize( new Dimension(320, 22));
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.RELATIVE;
		gbl.setConstraints(pathField, gbc);
		add(pathField);

		browseButton = new JButton(ResourceUtil.getRS( "dialog.browse"));
		gbc.weightx = 0;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints(browseButton, gbc);
		browseButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				browseFile();
			}
		});
		add(browseButton);
	}

	private void browseFile()
	{
		JFileChooser chooser =
			new JFileChooser(OBEDesigner.getWorkingDirectory());
		chooser.setFileFilter(new DirectoryFileFilter());
		chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		chooser.rescanCurrentDirectory();
		if (chooser.showDialog(this, ResourceUtil.getRS("file.select"))
			== JFileChooser.APPROVE_OPTION)
		{
			try
			{
				File selectedFile = chooser.getSelectedFile();
				this.pathField.setText(chooser.getSelectedFile().toString());
			}
			catch (Throwable t)
			{
				t.printStackTrace();
			}
		}
	}
	
	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	class DirectoryFileFilter extends FileFilter
	{
		public DirectoryFileFilter()
		{}
		public boolean accept(File file)
		{
			if (file.isDirectory())
				return true;
			else
				return false;
		}
		public String getDescription()
		{
			return ResourceUtil.getRS("file.dir");
		}
	}
	
}
