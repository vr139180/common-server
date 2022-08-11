package com.cms.designer.workflow.dialog;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.BasicWorkflowProcess;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.Utilities;
import com.cms.designer.workflow.editor.EntityEditor;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: NewWorkflowProcessDialog.java,v 1.3 2004/08/16 06:24:22 jefferyd Exp $
 * $Id: NewWorkflowProcessDialog.java,v 1.3 2004/08/16 06:24:22 jefferyd Exp $
 */
public class NewWorkflowProcessDialog extends JDialog
{
	private static final Log log = LogFactory.getLog(NewWorkflowProcessDialog.class);
	public static final int APPROVE_OPTION = 1;
	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	private OBEDesigner parent;

	private EntityEditor entityEditor;

	private JButton okButton;
	private JButton cancelButton;
	
	private WorkflowProcess wp;

	public NewWorkflowProcessDialog(OBEDesigner parent)
	{
		super(parent, true);
		this.parent = parent;
		init();
	}

	public WorkflowProcess getWorkflowProcess()
	{
		return wp;
	}

	public int showDialog()
	{
		Utilities.center(this);
		setVisible(true);
		return result;
	}

	public boolean save()
	{
		if( !entityEditor.save())
			return false;

		if(wp.getName()==null||wp.getName().trim().length()<=0){
			JOptionPane.showConfirmDialog(null,"请填写名称.","名称不能为空.",JOptionPane.DEFAULT_OPTION);
			return false;
		}
		return true;
	}

	public void close()
	{
		dispose();
	}

	private void init()
	{
		//初始化流程信息,process的id是确定的
		BasicWorkflowProcess bwp = new BasicWorkflowProcess( "none", ResourceUtil.getRS("window.newprocess"));
		bwp.initBasicWorkflowProcess();
		wp =bwp;

		setModal(true);
		setResizable( false);
		getContentPane().setLayout(new BorderLayout());
		setTitle(ResourceUtil.getRS("window.newWorkflowProcess"));

		JTabbedPane tabbedPane = new JTabbedPane();
		getContentPane().add(tabbedPane, BorderLayout.CENTER);

		entityEditor = new EntityEditor();
		tabbedPane.add( "基本信息", entityEditor);

		getContentPane().add( createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton(okButton);

		//设置修改关联
		entityEditor.setElement( wp);

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
