package com.cms.designer.workflow.jar;

import java.io.File;
import java.util.List;
import java.util.jar.Manifest;

/**
 * @author today
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class JarContent {
	
	private String deployDir;

    private int defaultLevel=7;    
	
	private List filesEntry;
	
	private List DirectoryEntry;

    /** Manifest instance */
    private Manifest manifest;
    
    /** target file for archive */
    private String targetFile;
    
    
	private File[] files;
	/**
	 * 
	 */
	public JarContent() {
		super();
		// TODO Auto-generated constructor stub
	}
 
    public String[] getFiles()
    {
     	return (String[])filesEntry.toArray();
    }
    
    public  String[] getdirectorys()
    {
		return (String[])DirectoryEntry.toArray();
    }
    
	public void putFile (File file) {
			filesEntry.add(file.getAbsolutePath());
		} 
		
	/** Inserts specified array of file objects into the
	 * jar content.
	 */
	public void putFiles (File[] files) {
	  for (int i = 0; i < files.length; i++) {
		filesEntry.add(files[i].getAbsolutePath());
	  }
	}


	/** Removes specified file object from the archive content
	 */
	public void removeFile (File file) {
		filesEntry.remove(file.getAbsolutePath());
	}

	/** Removes specified array of file objects from the archive content
	 */
	public void removeFiles (File[] files) {
	  for (int i = 0; i < files.length; i++) {
		removeFile(files[i]);
	  }
	}
	
	public void putDirectory (File dirName) {
		DirectoryEntry.add(dirName.getAbsolutePath());
	} 
		
	/** Inserts specified array of directoryobjects into the
	 * jar content.
	 */
	public void putDirectorys (File[] dirNames) {
	  for (int i = 0; i < dirNames.length; i++) {
		DirectoryEntry.add(dirNames[i].getAbsolutePath());
	  }
	}


	/** Removes specified directory object from the archive content
	 */
	public void removeDirectory (File dirName) {
		DirectoryEntry.remove(dirName.getAbsolutePath());
	}

	/** Removes specified array of directory objects from the archive content
	 */
	public void removeDirectorys (File[] dirNames) {
	  for (int i = 0; i < dirNames.length; i++) {
		removeDirectory(dirNames[i]);
	  }
	}

	/**
	 * @return
	 */
	public String getDeployDir() {
		return deployDir;
	}

	/**
	 * @return
	 */
	public Manifest getManifest() {
		return manifest;
	}

	/**
	 * @return
	 */
	public String getTargetFile() {
		return targetFile;
	}

	/**
	 * @param string
	 */
	public void setDeployDir(String string) {
		deployDir = string;
	}

	/**
	 * @param manifest
	 */
	public void setManifest(Manifest manifest) {
		this.manifest = manifest;
	}

	/**
	 * @param string
	 */
	public void setTargetFile(String string) {
		targetFile = string;
	}

}
