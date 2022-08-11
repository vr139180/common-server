package com.cms.designer.workflow.editor;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.designer.swingui.editor.ImplementationEditor;

public interface WorkflowEditor extends ImplementationEditor
{
	public void setWorkflowPackage(WorkflowPackage workflowPackage);
}
