package com.cms.designer.workflow.jar;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import java.util.Vector;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Iterator;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarInputStream;
import java.util.jar.JarOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * @author Today
 * 
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class JarUtil
{
	public static String getAppDescOfJar( File jar, String entryName)
	{
		String ret =null;
		JarFile jarFile = null;
		try
		{
			jarFile = new JarFile( jar);
			JarEntry en =jarFile.getJarEntry( entryName);
			if( en != null)
			{
				ByteArrayOutputStream out =new ByteArrayOutputStream();
				InputStream in =jarFile.getInputStream( en);
				int read =0;
				byte[] bb =new byte[512];
				while( (read =in.read( bb)) != -1)
				{
					out.write( bb, 0, read);
				}
				
				ret =out.toString();
				out.close();
			}
			jarFile.close();
		}
		catch( Exception ex)
		{
			try
			{
				jarFile.close();
			}
			catch( Exception e)
			{
			}
			
			ret =null;
		}

		return ret;
	}
		
	public static Map getJarFileEntries( String fileName) throws IOException
	{
		if( StringUtil.isEmpty( fileName))
			return null;
		Map map = new HashMap();
		JarFile jarFile = null;
		try
		{
			jarFile = new JarFile( fileName);
			int size = jarFile.size();
			Enumeration entries = jarFile.entries();
			while( entries.hasMoreElements())
			{
				JarEntry entry = (JarEntry) entries.nextElement();
				map.put( entry.getName(), entry);
			}
			jarFile.close();
		}
		catch( Exception ex)
		{
			try
			{
				jarFile.close();
			}
			catch( Exception e)
			{
			}
			throw new IOException( ex.getMessage());
		}
		return map;
	}

	public static JarEntry[] getJarFileEntriesByArray( String fileName) throws IOException
	{
		JarEntry[] entrys;
		if( StringUtil.isEmpty( fileName))
			return null;
		
		Map map = new HashMap();
		JarFile jarFile = null;
		try
		{
			jarFile = new JarFile( fileName);
			List ll =new ArrayList();
			Enumeration entries = jarFile.entries();
			while( entries.hasMoreElements())
			{
				JarEntry entry = (JarEntry) entries.nextElement();
				if( entry == null)
					continue;
				ll.add( entry);
			}
			jarFile.close();
			entrys =( JarEntry[])ll.toArray();
		}
		catch( Exception ex)
		{
			try
			{
				jarFile.close();
			}
			catch( Exception e)
			{
			}
			throw new IOException( ex.getMessage());
		}
		return entrys;
	}

	// Create a new jar file
	public static void newJar( String fileName) throws IOException
	{
		if( fileName == null || fileName.length() == 0)
			return;
		JarOutputStream jout = null;
		File tmpjar = null;
		try
		{
			// Create new zip file
			tmpjar = new File( fileName);
			// Create an empty entry and put it into the jar file
			JarEntry entry = new JarEntry( ".");
			jout = new JarOutputStream( new FileOutputStream( tmpjar));
			jout.putNextEntry( entry);
			jout.closeEntry();
			jout.close();
		}
		catch( IOException ex)
		{
			try
			{
				jout.close();
				tmpjar.delete();
			}
			catch( Exception ex1)
			{
			}
			throw ex;
		}
	}

	public static void addFiles( String jarFileName, String baseDir, String[] fileNames, int level,
			boolean replace) throws IOException
	{
		if( StringUtil.isEmpty( jarFileName))
			return;
		if( fileNames == null || fileNames.length <= 0)
			return;

		// Use a map to facilitate the searching operation
		Map filesMap = new HashMap();
		for( int i = 0; i < fileNames.length; i++)
		{
			String fileName = fileNames[i];
			if( fileName != null)
			{
				if( fileName.equalsIgnoreCase( jarFileName)){ throw new IOException( "无法添加文件到自身"); }
				if( fileName.equalsIgnoreCase( jarFileName + ".tmp")){ throw new IOException( 
						"无法添加程序将要创建的临时文件"); }
				filesMap.put( StringUtil.getEntryName( baseDir, fileName), fileName);
			}
		}

		level = checkJarLevel( level);
		JarOutputStream jout = null;
		JarInputStream jin = null;
		JarFile jarFile = null;
		File tmpjar = null;
		try
		{
			jarFile = new JarFile( jarFileName);
			// Create temporary jar file

			tmpjar = new File( jarFileName + ".tmp");
			JarEntry entry;
			jin = new JarInputStream( new FileInputStream( jarFileName));
			jout = new JarOutputStream( new FileOutputStream( tmpjar));
			jout.setLevel( level);
			int len = 0;
			byte[] b = new byte[10240];// 10KB buffer
			// Copy the old zip entries
			while( (entry = jin.getNextJarEntry()) != null)
			{

				// Find out if the replaced file is found
				if( replace == true && filesMap.get( entry.getName()) != null)
				{
					continue;
				}
				jout.putNextEntry( createEntry( entry));
				while( (len = jin.read( b)) != -1)
				{
					jout.write( b, 0, len);
				}
				jout.closeEntry();
				jin.closeEntry();
			}

			// Add the specified files into the zip file
			for( int i = 0; i < fileNames.length; i++)
			{
				String fileName = fileNames[i];
				String entryName = StringUtil.getEntryName( baseDir, fileName);
				if( (jarFile.getEntry( entryName) == null)
						|| ((jarFile.getEntry( entryName) != null && replace)))
				{
					File addedFile = new File( fileName);
					entry = new JarEntry( entryName);
					entry.setMethod( JarEntry.DEFLATED);
					entry.setTime( addedFile.lastModified());

					// Directory only put the entry, not write any data
					if( addedFile.isDirectory())
					{
						jout.putNextEntry( entry);
						continue;
					}
					FileInputStream in = new FileInputStream( addedFile);
					entry.setSize( in.available());
					jout.putNextEntry( entry);
					while( (len = in.read( b)) != -1)
					{

						jout.write( b, 0, len);
					}
					in.close();
					jout.closeEntry();
				}
			}

			jout.close();
			jin.close();
			jarFile.close();
			File oldFile = new File( jarFileName);
			oldFile.delete();
			tmpjar.renameTo( oldFile);
		}
		catch( IOException ex)
		{
			// Delete the temporary file
			try
			{
				jin.close();
				jout.close();
				jarFile.close();
				tmpjar.delete();
			}
			catch( Exception ex1)
			{
			}
			throw new IOException( ex.getMessage());
		}
	}

	public static int checkJarLevel( int level)
	{
		if( level < 0 || level > 9)
			level = 7;
		return level;
	}

	/**
	 * Add a directory's content to the zip file.
	 */
	public static void addDir( String jarFileName, String dirToAdd, int level,
			boolean recurseSubFolders, boolean replace) throws IOException
	{
		if( StringUtil.isEmpty( jarFileName) || StringUtil.isEmpty( dirToAdd)){ return; }
		try
		{
			File addedFile = new File( dirToAdd);
			if( !addedFile.isDirectory()){ throw new IOException( "此文件不是目录"); }

			if( !addedFile.exists()){ throw new IOException( "此目录不存在"); }

			// Get all the files
			Map map = scanFilesInDir( dirToAdd, recurseSubFolders, null);
			Object[] files = map.values().toArray();
			String[] fileNames = new String[files.length];
			for( int i = 0; i < files.length; i++)
			{
				fileNames[i] = files[i] + "";
			}

			addFiles( jarFileName, dirToAdd, fileNames, level, replace);
		}
		catch( Exception ex)
		{
			throw new IOException( ex.getMessage());
		}
	}

	/**
	 * Add a dir to the zip file, use base dir name, both the two path name can
	 * be relative or absolute path, the first parameter must be same level as
	 * the base dir or be the base dir's child.
	 * 
	 * @param recurseSubFolders -
	 *            whether or not recurse to sub folders
	 */
	private static Map scanFilesInDir( String baseDir, boolean recurseSubFolders, Map map)
			throws Exception
	{
		File base = new File( baseDir);
		if( !base.exists())
			throw new IOException( "目录不存在!");
		String fileName = base.getCanonicalPath();

		// Make sure the directory name ends with file separator
		if( base.isDirectory())
		{
			if( !fileName.endsWith( File.separator))
			{
				fileName += File.separator;
			}
		}

		if( map == null)
		{
			map = new HashMap();
		}
		map.put( fileName, fileName);

		if( base.isFile()){ return map; }

		// Do not recurse subfolders' file, only one level, then add files in
		// this directory
		if( !recurseSubFolders && base.isDirectory())
		{
			File list[] = base.listFiles();
			for( int i = 0; i < list.length; i++)
			{
				fileName = list[i].getCanonicalPath();
				map.put( fileName, fileName);
			}
			return map;
		}
		else if( base.isDirectory())
		{
			File list[] = base.listFiles();
			for( int i = 0; i < list.length; i++)
			{
				scanFilesInDir( list[i].getCanonicalPath(), true, map);
			}
		}
		return map;
	}

	public static void deleteTempfile( String jarFileName, int level) throws IOException
	{
		JarEntry[] entrys = new JarEntry[1];
		entrys[0] = new JarEntry( ".");
		JarUtil.deleteFiles( jarFileName, entrys, 7);
	}

	// Delete all the selected entries
	public static void deleteFiles( String jarFileName, JarEntry[] selectedEntries, int level)
			throws IOException
	{
		if( StringUtil.isEmpty( jarFileName))
			return;
		if( selectedEntries == null || selectedEntries.length == 0)
			return;
		level = checkJarLevel( level);

		if( getEntrySize( jarFileName) < 1){ return; }
		boolean isDeleteAll = (selectedEntries.length == getEntrySize( jarFileName));

		HashMap map = new HashMap();
		for( int i = 0; i < selectedEntries.length; i++)
		{
			map.put( selectedEntries[i].getName(), selectedEntries[i]);
		}

		JarOutputStream jout = null;
		JarInputStream jin = null;
		File tmpjar = null;

		try
		{
			// Create temporary zip file
			tmpjar = new File( jarFileName + ".tmp");
			JarEntry entry;
			jin = new JarInputStream( new FileInputStream( jarFileName));
			jout = new JarOutputStream( new FileOutputStream( tmpjar));
			jout.setMethod( JarOutputStream.DEFLATED);
			jout.setLevel( level);
			int len = 0;
			byte[] b = new byte[10240];
			while( (entry = jin.getNextJarEntry()) != null)
			{

				if( map.get( entry.getName()) != null)
				{
					continue;
				}

				jout.putNextEntry( createEntry( entry));

				while( (len = jin.read( b)) != -1)
				{
					jout.write( b, 0, len);
				}
				jout.closeEntry();
				jin.closeEntry();
			}
			// Check if there's no entry in the file,
			// else create an empty entry and put it into the zip file
			if( isDeleteAll)
			{
				entry = new JarEntry( ".");
				jout.putNextEntry( entry);
				jout.closeEntry();
			}

			jout.close();
			jin.close();
			File oldFile = new File( jarFileName);
			oldFile.delete();
			tmpjar.renameTo( oldFile);
		}
		catch( Exception ex)
		{
			// Delete the temporary file
			try
			{
				jin.close();
				jout.close();
				tmpjar.delete();
			}
			catch( Exception ex1)
			{
			}
			throw new IOException( ex.getMessage());
		}
	}
	
	public static void extractJar( File jar, File dir)
	{
		FileInputStream in1 =null;
		InputStream in =null;
		try
		{
			in1 =new FileInputStream( jar);
			in =new BufferedInputStream( in1);
	
			ZipInputStream zis = new ZipInputStream( in);
			ZipEntry e;
			while( (e = zis.getNextEntry()) != null)
			{
				String nn =e.getName().toLowerCase();
				if( !nn.endsWith( ".xml") && !nn.endsWith( ".jar"))
					continue;
				
				File f = new File( dir, e.getName().replace( '/', File.separatorChar));
				if( e.isDirectory())
				{
					//建立目录
					if( !f.exists() && !f.mkdirs() || !f.isDirectory())
						throw new IOException( "Can't create directory " + f.getPath());
				}
				else
				{
					if( f.getParent() != null)
					{
						File d = new File( f.getParent());
						if( !d.exists() && !d.mkdirs() || !d.isDirectory())
							throw new IOException( "Can't create directory " + d.getPath());
					}
	
					//释放文件
					OutputStream os = new FileOutputStream( f);
					byte[] b = new byte[512];
					int len;
					while( (len = zis.read( b, 0, b.length)) != -1)
					{
						os.write( b, 0, len);
					}
					zis.closeEntry();
					os.close();
				}
			}
		}
		catch( Exception ee)
		{
		}
		finally
		{
			try
			{
				if( in1 != null)
					in1.close();
				if( in != null)
					in.close();
			}
			catch( Exception ee)
			{
			}
		}
	}
	
	public static void extractFiles( String jarFileName, String selectedEntrie,
			String extractDir) throws IOException{
		boolean usePath=false;
		boolean replace=true;
		JarEntry[] selectedEntries=new JarEntry[1];
		selectedEntries[0]=new JarEntry(selectedEntrie);
		extractFiles(  jarFileName,  selectedEntries,
				 extractDir,  usePath,  replace);
	}
	
	public static void extractFiles( String jarFileName, String selectedEntrie,
			String extractDir, boolean usePath, boolean replace) throws IOException{
		JarEntry[] selectedEntries=new JarEntry[1];
		selectedEntries[0]=new JarEntry(selectedEntrie);
		extractFiles(  jarFileName,  selectedEntries,
				 extractDir,  usePath,  replace);
	}
	
	public static void extractFiles( String jarFileName, JarEntry selectedEntrie,
			String extractDir, boolean usePath, boolean replace) throws IOException{
		JarEntry[] selectedEntries=new JarEntry[1];
		selectedEntries[0]=selectedEntrie;
		extractFiles(  jarFileName,  selectedEntries,
				 extractDir,  usePath,  replace);
	}
	
	public static void extractFiles( String jarFileName, JarEntry[] selectedEntries,
			String extractDir, boolean usePath, boolean replace) throws IOException
	{
		if( StringUtil.isEmpty( jarFileName))
			return;
		if( StringUtil.isEmpty( extractDir))
			return;
		if( selectedEntries == null || selectedEntries.length == 0)
			return;

		JarFile jarFile = null;

		try
		{
			jarFile = new JarFile( jarFileName);
			// Make sure the directory name ends with a file separator
			if( !extractDir.endsWith( File.separator))
			{
				extractDir += File.separator;
			}
			// Check if the extract target directory exists, if not, create it
			File extractDirFile = new File( extractDir);
			if( !extractDirFile.exists())
			{
				extractDirFile.mkdirs();
			}

			for( int i = 0; i < selectedEntries.length; i++)
			{
				String entryName = selectedEntries[i].getName();

				JarEntry entry = jarFile.getJarEntry( entryName);
				// Check and create the directory contained in the entry
				// Create neccesary directroy
				if( usePath)
				{
					String entryDir = StringUtil.getDirName( entryName);
					if( entryDir == null)
						entryDir = "";
					File tempDir = new File( extractDir + entryDir);
					if( !tempDir.exists())
					{
						tempDir.mkdirs();
					}
				} // User not want create directory
				else
					entryName = StringUtil.getFileName( entryName);
				if( !entry.isDirectory())
				{
					File outFile = new File( extractDir + entryName);
					outFile.setLastModified( entry.getTime());
					// If file is directory or file exists and not replace it,
					// then jumb it
					if( outFile.isDirectory() || (outFile.exists() && replace == false))
					{
						continue;
					}
					InputStream in = jarFile.getInputStream( entry);
					// Burrer size: 10KB
					BufferedOutputStream out = new BufferedOutputStream( new FileOutputStream( 
							outFile), 10240);
					int data;
					while( (data = in.read()) != -1)
					{
						out.write( data);
					}
					in.close();
					out.close();
				}
			}
			jarFile.close();
		}
		catch( Exception ex)
		{
			try
			{
				jarFile.close();
			}
			catch( Exception ex1)
			{
			}
			throw new IOException( ex.getMessage());
		}
	}

	public static void extractFiles( String jarFileName, String extractDir, boolean usePath,
			boolean replace) throws IOException
	{
		JarEntry[] selectedEntries = getJarFileEntriesByArray( jarFileName);
		extractFiles( jarFileName, selectedEntries, extractDir, usePath, replace);
	}

	public static void renameEntry( String jarFileName, JarEntry oldEntry, JarEntry newEntry,
			int level) throws IOException
	{
		if( StringUtil.isEmpty( jarFileName))
			return;
		if( oldEntry == null || newEntry == null)
			return;
		// Same name and same time
		if( oldEntry.getName().equalsIgnoreCase( newEntry.getName()))
		{
			if( oldEntry.getTime() == newEntry.getTime()){ return; }
		}

		level = checkJarLevel( level);
		JarOutputStream jout = null;
		JarInputStream jin = null;
		File tmpjar = null;
		try
		{
			// Create temporary jar file
			tmpjar = new File( jarFileName + ".tmp");
			JarEntry entry;
			jin = new JarInputStream( new FileInputStream( jarFileName));
			jout = new JarOutputStream( new FileOutputStream( tmpjar));
			jout.setLevel( level);
			int len = 0;
			byte[] b = new byte[10240];// 10KB buffer
			// Copy the old zip entries
			while( (entry = jin.getNextJarEntry()) != null)
			{

				// Find the entry to update
				if( entry.getName().equals( oldEntry.getName()))
				{
					entry = createNewEntry( oldEntry, newEntry);
				}
				jout.putNextEntry( createEntry( entry));
				while( (len = jin.read( b)) != -1)
				{
					jout.write( b, 0, len);
				}
				jout.closeEntry();
				jin.closeEntry();
			}

			jout.close();
			jin.close();
			File oldFile = new File( jarFileName);
			oldFile.delete();
			tmpjar.renameTo( oldFile);
		}
		catch( IOException ex)
		{
			// Delete the temporary file
			try
			{
				jin.close();
				jout.close();
				tmpjar.delete();
			}
			catch( Exception ex1)
			{
			}
			throw new IOException( ex.getMessage());
		}
	}

	/**
	 * Because sometimes found compressedSize errors when directly put old
	 * entry from WinZip created files to ZipOutputStream, so change it.
	 */
	private static JarEntry createNewEntry( JarEntry oldEntry, JarEntry newEntry)
	{
		if( oldEntry.getMethod() == JarEntry.STORED)
		{
			// Stored entry directly copy all the data
			JarEntry entry = new JarEntry( newEntry.getName());
			entry.setMethod( oldEntry.getMethod());
			entry.setTime( oldEntry.getTime());
			entry.setExtra( oldEntry.getExtra());
			entry.setComment( oldEntry.getComment());
			entry.setCompressedSize( oldEntry.getCompressedSize());
			entry.setCrc( oldEntry.getCrc());
			entry.setSize( oldEntry.getSize());
			return entry;
		}
		else if( oldEntry.getMethod() == JarEntry.DEFLATED)
		{
			// Deflated entry not set size and compressedSize
			JarEntry entry = new JarEntry( newEntry.getName());
			entry.setMethod( oldEntry.getMethod());
			entry.setTime( oldEntry.getTime());
			entry.setExtra( oldEntry.getExtra());
			entry.setComment( oldEntry.getComment());
			return entry;
		}
		return oldEntry;
	}

	/**
	 * Because sometimes found compressedSize errors when directly put old
	 * entry from WinZip created files to ZipOutputStream, so change it.
	 */
	private static JarEntry createEntry( JarEntry oldEntry)
	{
		if( oldEntry.getMethod() == JarEntry.STORED)
		{
			// Stored entry directly put it out
			return oldEntry;
		}
		else if( oldEntry.getMethod() == JarEntry.DEFLATED)
		{
			// Deflated entry not set size and compressedSize
			JarEntry entry = new JarEntry( oldEntry.getName());
			entry.setMethod( oldEntry.getMethod());
			entry.setTime( oldEntry.getTime());
			entry.setExtra( oldEntry.getExtra());
			entry.setComment( oldEntry.getComment());
			return entry;
		}
		return oldEntry;
	}

	// Get the entry number in a jar file
	public static int getEntrySize( String jarFileName)
	{
		JarFile jarFile = null;
		int size = -1;
		try
		{
			jarFile = new JarFile( jarFileName);
			size = jarFile.size();
			jarFile.close();
		}
		catch( IOException ex)
		{
			try
			{
				jarFile.close();
			}
			catch( Exception e)
			{
			}
		}
		return size;
	}
	
	public static List getFileEndWith(String jarFileName,String str) throws IOException
	{
		Vector vFile = new Vector();
		Map map = getJarFileEntries( jarFileName);
		Iterator iterator = map.keySet().iterator();
		while (iterator.hasNext() )
		{
			String name = (String)iterator.next();
			if (name.endsWith(str))  
				vFile.add(name);
		}
		return vFile;
	}
	/**
	 * Jar包中存在改文件
	 * @param jarFileName
	 * @param str
	 * @return
	 * @throws IOException
	 */
	public static boolean hasFileInJar(String jarFileName,String str) throws IOException{
		
		Map map = getJarFileEntries(jarFileName);
		Iterator iterator = map.keySet().iterator();
		while (iterator.hasNext() )
		{
			String name = (String)iterator.next();
			name = StringUtil.getFileName(name);
			if (name.equals(str))  return true;
		}
		return false;
	}
	/**
	 * jar中存在该类
	 * @param jarFileName
	 * @param str
	 * @return
	 * @throws IOException
	 */
	public static boolean hasClassInJar(String jarFileName,String classname) throws IOException{
		                                               
		char a = ".".toCharArray()[0];
		char b = "/".toCharArray()[0];//JAR的目录分割符号为"/" 不可用 file.separator代替
		classname = classname.replace(a,b);
		classname = classname + ".class";
		Map map = getJarFileEntries(jarFileName);
		Iterator iterator = map.keySet().iterator();
		while (iterator.hasNext() )
		{
			String name = (String)iterator.next();
			if (name.equals(classname))  return true;
		}
		return false;
	}
	
}
