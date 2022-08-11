package com.cms.designer.workflow.jar;

import java.io.File;
import java.io.IOException;
/**
 * @author today
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class JarCreateor implements PluginNames
{
	private String deployDir;
	private int defaultLevel = 7;

	private File[] files;
	private String[] fileNames;

	private File dirFile;
	private String dirString;

	public void addAllEntry(String targetFile) throws IOException
	{
		JarUtil.newJar( targetFile);
		
		addXpdlDirEntry( targetFile);
		
		delTempEntry(targetFile);
	}

	public void createAll() throws IOException
	{
		createXpdlDir();
	}
	
	public void delAll() throws IOException
	{
		delXpdlDir();
		
		deleteDirectory( new File( deployDir));
	}

	private void addXpdlDirEntry(String targetFile) throws IOException
	{
		dirFile = new File(deployDir + File.separator + WORKFLOW_DIR);
		if (isDirExist(dirFile))
		{
			dirString = dirFile.getAbsolutePath();
			JarUtil.addDir(targetFile, dirString, defaultLevel, true, true);
		}

	}

	private void delTempEntry(String targetFile) throws IOException
	{
		JarUtil.deleteTempfile(targetFile, defaultLevel);
	}

	private boolean isFileExist(String fileName)
	{
		return new File(fileName).isFile();
	}

	private boolean isFileExist(File fileName)
	{
		return fileName.isFile();
	}

	private boolean isDirExist(String dirName)
	{
		return new File(dirName).isDirectory();
	}

	private boolean isDirExist(File dirName)
	{
		return dirName.isDirectory();
	}

	private void createXpdlDir()
	{
		dirFile = new File(deployDir + File.separator + WORKFLOW_DIR);
		if (!dirFile.exists())
			dirFile.mkdirs();
	}

	private boolean deleteDirectory(File dir) throws IOException
	{
		if ((dir == null) || !dir.isDirectory())
		{
			throw new IllegalArgumentException(
				"Argument " + dir + " is not a directory. ");
		}

		File[] entries = dir.listFiles();
		int sz = entries.length;

		for (int i = 0; i < sz; i++)
		{
			if (entries[i].isDirectory())
			{
				if (!deleteDirectory(entries[i]))
				{
					return false;
				}
			}
			else
			{
				if (!entries[i].delete())
				{
					return false;
				}
			}
		}

		if (!dir.delete())
		{
			return false;
		}
		return true;
	}

	private void delXpdlDir() throws IOException
	{
		dirFile = new File(deployDir + File.separator + WORKFLOW_DIR);
		if (isDirExist(dirFile))
		{
			deleteDirectory(dirFile);
		}
	}

	/**
	 * @return
	 */
	public String getDeployDir()
	{
		return deployDir;
	}

	/**
	 * @param string
	 */
	public void setDeployDir(String string)
	{
		deployDir = string;
	}

}
