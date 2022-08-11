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
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.designer.swingui.editor.ImplementationEditor;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.editor.ToolSetEditor;
import com.cms.designer.workflow.panel.ActivityNormalInformationPanel;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: EditActivityDialog.java,v 1.19 2004/10/10 09:17:52 york Exp $
 */
public class EditActivityDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(EditActivityDialog.class);
	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	private OBEGraphWorkflow parent;
	private ImplementationEditor implementationEditor;
	private String elementKey;

	private Activity activity;
	private WorkflowPackage wfPackage;
	private WorkflowProcess wfProcess;

	// 常规属性
	private ActivityNormalInformationPanel activityNormalInformationPanel;
	//应用设置
	private ToolSetEditor	toolAppPanel;
	
	public EditActivityDialog( OBEGraphWorkflow parent, String key, Activity a, 
		ImplementationEditor implementationEditor, WorkflowPackage wp, WorkflowProcess wfprocess)
	{
		super(parent.getDesignerParent(), true);
		
		this.elementKey =key;
		this.parent = parent;
		this.wfPackage =wp;
		this.wfProcess =wfprocess;
		
		this.activity = a;
		
		this.implementationEditor = implementationEditor;

		try
		{
			jbInit();
			revert();
		}
		catch (Exception e)
		{
			
		}
	}
	
	public boolean save()
	{
		String oldId = activity.getId();
	
		if (!activityNormalInformationPanel.save())
			return false;

		parent.updateTransitions( oldId, activity.getId());
		
		if( toolAppPanel != null && !toolAppPanel.save())
			return false;

		return true;
	}

	public void revert()
	{
		//常规信息
		activityNormalInformationPanel.setElement(activity);
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

	private void jbInit() throws Exception
	{
		this.setResizable( true);
		getContentPane().setLayout(new BorderLayout());
		setModal(true);
		setTitle( "设置Actions");

		//tab控件
		JTabbedPane tabbedPane = new JTabbedPane();
		getContentPane().add(tabbedPane, BorderLayout.CENTER);

		activityNormalInformationPanel = new ActivityNormalInformationPanel( elementKey, activity);
		tabbedPane.add(ResourceUtil.getRS("detail.general"), activityNormalInformationPanel);

		if( implementationEditor instanceof ToolSetEditor)
		{
			toolAppPanel =new ToolSetEditor();
			toolAppPanel.setOwner( this);
			toolAppPanel.set( parent.getFrame().getName(), wfProcess, activity);
			
			tabbedPane.add( "actions", toolAppPanel);
		}
		
		//增加功能按钮
		getContentPane().add(createButtonPanel(), BorderLayout.SOUTH);

		pack();
	}

	/**
	 * 创建工具条
	 * @return
	 */
	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		JButton okButton = new JButton(ResourceUtil.getRS("detail.ok"));
		panel.add(okButton);
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

		JButton cancelButton = new JButton(ResourceUtil.getRS("detail.cancel"));
		panel.add(cancelButton);
		cancelButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				result = CANCEL_OPTION;
				close();
			}
		});

		return panel;
	}

	public Activity getActivity()
	{
		return activity;
	}
}
