package com.cms.designer.workflow.editor;

import java.awt.Component;
import java.awt.Dialog;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.Implementation;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.core.workflow.activity.Tool;
import com.cms.core.workflow.activity.ToolSet;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.dialog.ToolDialog;

public class ToolSetEditor extends JPanel implements WorkflowEditor
{
	private Dialog owner;

	private JList objectList;

	private WorkflowProcess process;
	
	private Activity activity;
	
	private ToolSet toolSet;
	
	public ToolSetEditor()
	{	
		init();
	}
	
    public void set( String key, WorkflowProcess process, Activity act)
    {
		//初始化项目中可用的应用
		ElementKey ek =new ElementKey();
		ek.analyze( key);
		
		ProjectManager manager =OBEModuleManager.getInstance().getProjectManager();
		ProjectData pd =manager.getOneProject( ek.getProjectID());
		if( pd == null)	return;
		String uid =pd.getModuleUUID( OBEWorkflowModule.MODULE_ID);
		
		OBEWorkflowModule module =( OBEWorkflowModule)OBEModuleManager.getInstance().getModuleManager().getModule( OBEWorkflowModule.MODULE_ID);
		
		if( module == null)	return;
		
		WorkflowModuleData data =( WorkflowModuleData)module.getModuleData( uid);
		if( data == null)	return;

    	this.process = process;
    	this.activity = act;
    	this.toolSet = act.getTset_();
    	
    	updateListView();
    }

    private void updateListView()
    {
    	//初始化已设置信息
		Iterator iter = toolSet.getTools().values().iterator();
		Vector vt =new Vector();
		while( iter.hasNext())
		{
			Tool tool = (Tool) iter.next();
			vt.add( tool);
		}
		
		objectList.removeAll();
		objectList.setListData( vt);
    }
    
    public String getTitle()
	{
		return ResourceUtil.getRS("toolSet.toolSet");
	}

	public Component getComponent()
	{
		return this;
	}

	public boolean save()
	{
		return true;
	}

	public void revert()
	{}

	public void setWorkflowPackage(WorkflowPackage workflowPackage)
	{
	}

	public void setImplementation( Implementation implementation)
	{
	}

	public Implementation getImplementation()
	{
		return this.toolSet;
	}

	public void add()
	{
		Tool tool =new Tool( "");
		ToolDialog d = new ToolDialog( owner, process, toolSet, tool, true);

		d.setTitle( "action");
		if( d.showDialog() == ToolDialog.APPROVE_OPTION)
		{
			toolSet.addTools( tool);
			updateListView();
		}
	}

	public void edit()
	{
		Tool tool =(Tool) objectList.getSelectedValue();
		if( tool == null)
			return;
		
		tool =toolSet.getTool( tool.getName());
		if( tool == null)
			return;

		ToolDialog d = new ToolDialog( owner, process, toolSet, tool, false);
		if( d.showDialog() == ToolDialog.APPROVE_OPTION)
		{
		}
	}

	public void remove()
	{
		Tool tool =(Tool) objectList.getSelectedValue();
		if( tool != null && toolSet.removeTool( tool.getName()) != null)
		{
			updateListView();
		}
	}

	private void init()
	{
		GridBagLayout gbl = new GridBagLayout();
		GridBagConstraints gbc = new GridBagConstraints();
		setLayout( gbl);

		gbc.insets = new Insets( 1, 1, 1, 1);
		gbc.fill = GridBagConstraints.BOTH;
		gbc.anchor = GridBagConstraints.NORTHWEST;

		objectList = new JList();
		objectList.setCellRenderer( new DefaultListCellRenderer());
		JScrollPane scrollPane = new JScrollPane( objectList);
		gbc.gridwidth = 1;
		gbc.gridheight = 3;
		gbc.weightx = 1;
		gbc.weighty = 1;
		gbl.setConstraints( scrollPane, gbc);
		add( scrollPane);

		gbc.weightx = 0;
		gbc.weighty = 0;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbc.gridheight = 1;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		JButton addButton = new JButton( ResourceUtil.getRS( "participant.add"));
		addButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				add();
			}
		});
		gbl.setConstraints( addButton, gbc);
		add( addButton);

		JButton editButton = new JButton( ResourceUtil.getRS( "participant.edit"));
		editButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				edit();
			}
		});
		gbl.setConstraints( editButton, gbc);
		add( editButton);

		JButton removeButton = new JButton( ResourceUtil.getRS( "participant.remove"));
		removeButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				remove();
			}
		});
		gbl.setConstraints( removeButton, gbc);
		add( removeButton);
	}

	/**
	 * @param owner The owner to set.
	 */
	public void setOwner( Dialog owner)
	{
		this.owner = owner;
	}
}
