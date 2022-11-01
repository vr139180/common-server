package com.cms.designer.workflow.action;

import java.awt.event.ActionEvent;
import java.beans.PropertyVetoException;
import java.io.File;


import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.action.OBEStandardAction;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.ProjectManager;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.coremodule.workspace.ProjectData;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.ConstValue;
import com.cms.designer.util.OBETreeData;
import com.cms.designer.validate.Validate;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.OBEWorkFlowFrame;
import com.cms.designer.workflow.OBEWorkflowModule;
import com.cms.designer.workflow.TreeNodeKey;
import com.cms.designer.workflow.WorkflowModuleData;
import com.cms.designer.workflow.dialog.JarSaveDialog;
import com.cms.designer.workflow.jar.JarClient;
import com.cms.designer.workflow.validate.Validator;

/**
 * <p>Title: </p>
 * <p>Description: 打包action</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author zagio
 * @version 1.0
 */
public class DeployProcess extends OBEStandardAction
{
	public DeployProcess(OBEDesigner parent)
	{
		super(parent);
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		OBETreeData data =( OBETreeData)parent.getTree().getCurSelectedNode().getUserObject();
		if( data == null)	return;
		ElementKey ek =new ElementKey();
		ek.analyze( data.getKey());
		TreeNodeKey tk =new TreeNodeKey();
		tk.analyee( ek.getModuleExtAtt());
		
		ProjectManager pm =OBEModuleManager.getInstance().getProjectManager();
		ProjectData pd =pm.getOneProject( ek.getProjectID());
		if( pd == null)	return;
		String lid =pd.getModuleUUID( OBEWorkflowModule.MODULE_ID);
		if( lid == null || lid.equals( ""))
			return;
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		OBEWorkflowModule wmodule =( OBEWorkflowModule)mm.getModule( OBEWorkflowModule.MODULE_ID);
		if( wmodule == null)
			return;
		//workflow模块数据
		WorkflowModuleData md =( WorkflowModuleData)wmodule.getModuleData( lid);
		
		if( md == null)
			return;
		WorkflowPackage wpackage =md.getOnePackage( tk.getProcessID());
		WorkflowProcess wprocess =md.getOneProcess( tk.getProcessID());
		if( wprocess == null)
			return;
		
		((WorkflowSaveAction)wmodule.getIDEAction( ConstValue.CoreFileSave)).actionPerformed(new ActionEvent("1",1,""));

		JarSaveDialog jsave =new JarSaveDialog( parent);
		if( jsave.showDialog() != JarSaveDialog.APPROVE_OPTION)
			return;

		//查找internalframe
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)wmodule.newModuleFrame( data.getKey());
		try
		{
		
			String n =wpackage.getName()+".svg";
			File file =new File( md.getWorkDir(), n);

			OBEGraphWorkflow graph =( OBEGraphWorkflow)frame.getGraph();
			
			JarClient jarClient =
				new JarClient( jsave.getDestPath(),jsave.getDestName(),	
						wpackage, wprocess,	file, frame.getName());
			jarClient.doAll();
		}
		catch (Exception ew)
		{
			ew.printStackTrace();
		}
		finally
		{
			try
			{
				if( frame != null)
					frame.setClosed( true);
			}
			catch( PropertyVetoException pv){
				pv.printStackTrace();
			}
		}
	}

	/**
	 * @param wp
	 * @return
	 */
	private boolean checkWorkflowValidate( ProjectData pd ,WorkflowModuleData data, 
			WorkflowPackage wp, WorkflowProcess wprocess)
	{
		boolean ret =false;

		Validate validate =new Validator( pd, data, wp, wprocess);
		ret =validate.validate();
		if( !ret)
			this.getParent().fireCheckToolTipInfo( true, validate);
		
		return ret;
	}
}