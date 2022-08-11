package com.cms.designer.workflow.dialog;

import java.awt.BorderLayout;
import java.awt.Dialog;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Tool;
import com.cms.core.workflow.activity.ToolSet;
import com.cms.designer.workflow.panel.ToolPanel;

/**
 * @author Administrator
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class ToolDialog extends JDialog
{
	public static final int APPROVE_OPTION = 3;

	public static final int CANCEL_OPTION = 2;

	private boolean bEdit =false;
	
	private WorkflowProcess process;
	private ToolSet toolSet;
	private Tool tool;
	
	private int result = CANCEL_OPTION;

	private JTabbedPane tabbedPane;

	private ToolPanel toolPanel;

	/**
	 * @param arg0
	 * @throws java.awt.HeadlessException
	 */
	public ToolDialog( Dialog arg0, WorkflowProcess process, 
			ToolSet toolSet, Tool tool, boolean bEdit) 
	throws HeadlessException
	{
		super( arg0);
    	this.process = process;
    	this.toolSet = toolSet;
    	this.tool = tool;
    	this.bEdit =bEdit;

		init();
	}
	
	public Tool getTool()
	{
		return tool;
	}

	public void setTool( Tool tool)
	{
		this.tool =tool;
	}
	
	private void init()
	{
		setModal( true);

		toolPanel = new ToolPanel( process, toolSet, tool);
		
		tabbedPane = new JTabbedPane();
		tabbedPane.add( ResourceUtil.getRS( "tool.tool"), new JScrollPane( toolPanel));

		getContentPane().setLayout( new BorderLayout());
		getContentPane().add( tabbedPane, BorderLayout.CENTER);
		getContentPane().add( createButtonPanel(), BorderLayout.SOUTH);

		setTitle( ResourceUtil.getRS( "toolSet.addTool"));
		pack();
	}

	private boolean save()
	{
		return this.toolPanel.save();
	}
	
	public int showDialog()
	{
		setVisible( true);
		return result;
	}

	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		JButton okButton = new JButton( ResourceUtil.getRS( "detail.ok"));
		okButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				if( save())
				{
					result = APPROVE_OPTION;
					close();
				}
			}
		});
		panel.add( okButton);

		JButton cancelButton = new JButton( ResourceUtil.getRS( "detail.cancel"));
		cancelButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				result = CANCEL_OPTION;
				close();
			}
		});
		panel.add( cancelButton);

		return panel;
	}

	public void close()
	{
		dispose();
	}
}
