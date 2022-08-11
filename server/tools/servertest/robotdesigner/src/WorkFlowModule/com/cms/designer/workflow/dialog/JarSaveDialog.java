package com.cms.designer.workflow.dialog;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.editor.JarSaveEditor;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class JarSaveDialog extends JDialog
{
	private static final Log log = LogFactory.getLog( JarSaveDialog.class);
	private OBEDesigner parent =null;

	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	//文件位置存放
	private JarSaveEditor editor;
	private String destPath ="";
	private String destName ="";
	
	private JButton okButton;
	private JButton cancelButton;

	public JarSaveDialog( OBEDesigner parent)
	{
		super(parent);
		this.parent = parent;
		init();
	}
	
	public int showDialog()
	{
		Utilities.center(this);
		setVisible(true);
		return result;
	}

	public void close()
	{
		dispose();
	}

	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		ModuleManager module =OBEModuleManager.getInstance().getModuleManager();
		okButton = new JButton( module.getResource( OBEWorkflowModule.MODULE_ID, "detail.ok"));
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				destName =editor.getDestName();
				if( destName == null || destName.equals( ""))
					return;
				destPath=editor.getDestPath();
				if( destPath == null || destPath.equals( ""))
					return;
				File ff =new File( destPath);
				if( !ff.exists() || !ff.isDirectory())
					return;
				
				File destFile=new File(destPath+File.separator+destName);
				if( destFile.exists())
				{	int flag=-1;
					flag = JOptionPane.showConfirmDialog( null, "要覆盖原目标文件吗?", "目标文件已存在!",JOptionPane.YES_OPTION);
					if( flag > 0){
						result =CANCEL_OPTION;
						close();
						return;
					}
				}

				result =APPROVE_OPTION;
				close();
			}
		});
		panel.add(okButton);

		cancelButton = new JButton( module.getResource( OBEWorkflowModule.MODULE_ID, "detail.cancel"));
		cancelButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				result = CANCEL_OPTION;
				close();
			}
		});
		panel.add(cancelButton);

		return panel;
	}

	private void init()
	{
		editor = new JarSaveEditor( parent);
		setModal(true);
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(editor, BorderLayout.CENTER);
		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);

		String t =OBEModuleManager.getInstance().getModuleManager().
				getResource( OBEWorkflowModule.MODULE_ID, "dialog.deployprocess");
		setTitle( t);
		pack();
	}
	/**
	 * @return
	 */
	public String getDestPath()
	{
		return destPath;
	}
	public String getDestName()
	{
		return destName;
	}

}
