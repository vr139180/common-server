package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.OBEModuleData;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBETreeNode;
import com.cms.designer.swingui.dialog.NewProjectDialog;
import com.cms.designer.util.OBETreeData;

/**
 * @author zagio
 * 创建新的项目
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class NewProject extends OBEStandardAction
{
	public NewProject(OBEDesigner parent)
	{
		super(parent);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		try
		{
			NewProjectDialog d = new NewProjectDialog(parent);
			if (d.showDialog() == NewProjectDialog.APPROVE_OPTION)
			{
				ProjectData project =d.getProjectData();
				//保存项目信息
				ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
				OBEModuleData md =pm.newProject( project);

				ElementKey ek =new ElementKey( project.getProjectID(), OBETreeNode.PROJECT, md.getModuleID(), md.getUuid());
				OBETreeData td =new OBETreeData( project.getProjectName(), OBETreeNode.PROJECT,
					"project.gif", ek.createKey());

				parent.getTree().addProjectNode( td);
				
				//保存项目信息到属性文件
				parent.getWorkSpaceManager().saveProjects();
			}
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}

}
