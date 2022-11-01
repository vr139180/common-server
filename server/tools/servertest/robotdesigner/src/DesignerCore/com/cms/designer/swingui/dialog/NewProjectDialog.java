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
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.editor.ProjectEditor;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class NewProjectDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(NewProjectDialog.class);
	private OBEDesigner parent =null;

	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	//项目编辑
	private ProjectEditor editor;

	private JButton okButton;
	private JButton cancelButton;

	public NewProjectDialog(OBEDesigner parent)
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

	/**
	 * @return
	 */
	public ProjectData getProjectData()
	{
		return editor.getProjectData();
	}

	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		okButton = new JButton(ResourceUtil.getRS("detail.ok"));
		okButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				if (save())
				{
					result = APPROVE_OPTION;
					close();
				}
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
		editor = new ProjectEditor( parent);
		setModal(true);
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(editor, BorderLayout.CENTER);
		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);

		setTitle(ResourceUtil.getRS("dialog.newProject"));
		pack();
	}

	private void revert()
	{}
}
