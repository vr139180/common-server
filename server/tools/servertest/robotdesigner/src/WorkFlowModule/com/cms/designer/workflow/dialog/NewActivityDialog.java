package com.cms.designer.workflow.dialog;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.editor.EntityEditor;
import com.cms.designer.workflow.editor.WorkflowEditor;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: NewActivityDialog.java,v 1.4 2004/08/10 08:20:31 jefferyd Exp $
 */
public class NewActivityDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(NewActivityDialog.class);
	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	private EntityEditor entityEditor;
	private WorkflowEditor implementationEditor;
	private String elementKey;

	private JButton okButton;
	private JButton cancelButton;

	private Activity activity;

	public NewActivityDialog( OBEGraphWorkflow parent, WorkflowEditor implementationEditor, 
			String name, String key)
	{
		super(parent.getDesignerParent(), true);
		this.implementationEditor = implementationEditor;
		this.elementKey =key;
		
		init();

		activity = new Activity( "needreplace", name != null ? name : "");
		entityEditor.setElement(activity);

		pack();
	}

	public NewActivityDialog(OBEGraphWorkflow parent, 
			WorkflowEditor implementationEditor, String key)
	{
		this(parent, implementationEditor, null, key);
	}

	public Activity getActivity()
	{
		return activity;
	}

	public int showDialog()
	{
		Utilities.center(this);
		setVisible(true);
		return result;
	}

	public boolean save()
	{
		if (!entityEditor.save())
			return false;

		return true;
	}

	public void close()
	{
		dispose();
	}

	private void init()
	{
		setModal(true);
		setTitle(ResourceUtil.getRS("activity.newActivity"));
		getContentPane().setLayout(new BorderLayout());

		JTabbedPane tabbedPane = new JTabbedPane();
		getContentPane().add(tabbedPane, BorderLayout.CENTER);

		entityEditor = new EntityEditor();
		tabbedPane.add(ResourceUtil.getRS("detail.general"), entityEditor);
		
		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);
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

}
