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
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.panel.ActivityNormalInformationPanel;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: EditWorkflowProcessDialog.java,v 1.7 2004/08/16 08:17:17 york Exp $
 */
public class EditWorkflowProcessDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(EditWorkflowProcessDialog.class);

	public static final int APPROVE_OPTION = 3;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	private OBEDesigner parent;
	private WorkflowProcess workflowProcess;

	private ActivityNormalInformationPanel activityNormalInformationPanel;
	
	private JButton okButton;
	private JButton cancelButton;

	//保存tree中的key
	private String elementKey;

	public EditWorkflowProcessDialog(OBEDesigner parent, WorkflowProcess workflowProcess, String elementKey, String title)
	{
		super(parent, true);
		setTitle( title);

		this.parent = parent;
		this.elementKey = elementKey;
		this.workflowProcess = workflowProcess;
		init();
		this.setWorkflowProcess(workflowProcess);
	}

	public void setWorkflowProcess(WorkflowProcess workflowProcess)
	{
		this.workflowProcess = workflowProcess;

		revert();
	}

	public WorkflowProcess getWorkflowProcess()
	{
		return workflowProcess;
	}

	public int showDialog()
	{
		Utilities.center(this);
		setVisible(true);
		return result;
	}

	public boolean save()
	{
		if (!activityNormalInformationPanel.save())
			return false;

		return true;
	}

	public void revert()
	{
	}

	public void close()
	{
		dispose();
	}

	private void init()
	{
		setModal(true);
		this.setResizable(false);
		getContentPane().setLayout(new BorderLayout());
		
		JTabbedPane tabbedPane = new JTabbedPane();
		getContentPane().add(tabbedPane, BorderLayout.CENTER);

		//常规
		activityNormalInformationPanel = new ActivityNormalInformationPanel(workflowProcess, elementKey, null);
		tabbedPane.add(ResourceUtil.getRS("detail.general"), activityNormalInformationPanel);

		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);

		pack();
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
