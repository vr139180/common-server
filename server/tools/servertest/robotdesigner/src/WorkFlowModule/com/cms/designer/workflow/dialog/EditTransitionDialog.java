package com.cms.designer.workflow.dialog;

import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.swingui.dialog.BasicDialog;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.conditionui.ConditionEditorPanel;
import com.cms.designer.workflow.editor.EntityEditor;

public class EditTransitionDialog extends BasicDialog
{
	private Transition transition;
	private WorkflowProcess wfprocess;

	private EntityEditor entityEditor;
	private ConditionEditorPanel conditionEditorPanel;

	public EditTransitionDialog( OBEGraphWorkflow parent, Transition transition)
	{
		super( parent.getDesignerParent(), true, BasicDialog.TYPE_FOUR);
		this.transition = transition;
		this.wfprocess =parent.getWorkflowProcess();
		
		init();
		revert();
	}

	public void addTabPanel()
	{
	}

	public boolean save()
	{
		if( !entityEditor.save())
			return false;
		if( conditionEditorPanel != null && !conditionEditorPanel.save())
		{
			return false;
		}
		
		return true;
	}

	public void revert()
	{
		entityEditor.setElement( transition);
		if( conditionEditorPanel != null)
		{
			conditionEditorPanel.setCondition( transition);
			conditionEditorPanel.setWfprocess( wfprocess);
		}
	}

	private void init()
	{
		setTitle( ResourceUtil.getRS( "transition.editTransition"));

		entityEditor = new EntityEditor();
		addPane( ResourceUtil.getRS( "detail.general"), entityEditor);
		
		conditionEditorPanel = new ConditionEditorPanel( this);
		addPane( ResourceUtil.getRS( "loop.condition"), conditionEditorPanel);

		pack();
		this.setSize( 500, 420);
	}
}
