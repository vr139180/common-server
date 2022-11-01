package com.cms.designer.swingui.dialog;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.editor.ProjectImportEditor;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 * $Id: ProjectImportDialog.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class ProjectImportDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(ProjectImportDialog.class);
	private OBEDesigner parent =null;
	
	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	//项目编辑
	private ProjectImportEditor editor;

	private JButton okButton;
	private JButton cancelButton;

	public ProjectImportDialog(OBEDesigner parent)
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

		okButton = new JButton(ResourceUtil.getRS("detail.ok"));
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				if( !editor.canSave())
					return;
				
				result = APPROVE_OPTION;
				close();
			}
		});
		panel.add(okButton);

		cancelButton = new JButton(ResourceUtil.getRS("detail.cancel"));
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

	/**
	 * @return
	 */
	public boolean save()
	{
		if (!editor.save())
			return false;
		return true;
	}

	private void init()
	{
		editor = new ProjectImportEditor(parent);
		setModal(true);
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(editor, BorderLayout.CENTER);
		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);

		setTitle(ResourceUtil.getRS("core.project.import"));
		pack();
	}

	private void revert()
	{}
}
