package com.cms.designer.workflow.jar;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.serializer.dom4j.Dom4JXPDLSerializer;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class JarClient implements PluginNames
{
	private String deployDir;
	private String targetFile;

	private String xpdlFile = "";

	private WorkflowPackage newPackage = null;
	private WorkflowProcess currentProcess = null;
	private JarCreateor jarCreateor = null;

	private String pkgId = "";
	private String pkgVersion = "";
	private String wfpId = "";
	private String wfpVeriosn = "";

	private String moduleKey;
	/**
	 * 
	 */
	public JarClient(
		String targetPath,
		String targetName,
		WorkflowPackage workflowPackage,
		WorkflowProcess workflowProcess,
		File svg,
		String key)
	{
		moduleKey =key;
		
		// 初始化数据
		deployDir = targetPath + File.separator + "__deploy";
		this.targetFile = targetPath + File.separator + targetName;

		xpdlFile = deployDir + File.separator + WORKFLOW_FILE;

		newPackage = workflowPackage;
		currentProcess = workflowProcess;

		jarCreateor = new JarCreateor();
		jarCreateor.setDeployDir(deployDir);

		pkgId = newPackage.getId();
		wfpId = currentProcess.getId();
	}

	public void doAll() throws IOException, Exception
	{
		//建立相关目录文件结构
		jarCreateor.createAll();

		writeXpdlFile();

		//打包
		jarCreateor.addAllEntry(targetFile);

		//删除文件目录 
		jarCreateor.delAll();
	}

	public void createStruct() throws IOException
	{}

	//	生成xpdl.xml文件
	public void writeXpdlFile() throws IOException
	{
		FileOutputStream out = null;
		try
		{
			out = new FileOutputStream(xpdlFile);
			List processes = newPackage.getWorkflowProcesses();

			processes.clear();
			processes.add(currentProcess);
			Dom4JXPDLSerializer serializer = new Dom4JXPDLSerializer();
			serializer.serialize(newPackage, out);
			out.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
			out.close();
		}
	}
	
	
}
