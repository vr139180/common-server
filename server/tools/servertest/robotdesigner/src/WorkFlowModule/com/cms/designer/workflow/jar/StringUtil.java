package com.cms.designer.workflow.jar;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;
import java.util.Date;
/**
 * @author Today
 * 
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class StringUtil
{
    public static List ArrayToList(String[] array)
    {
    	if(array==null||array.length==0) 
   			return null;

    	List v2 = new ArrayList();
		for (int i = 0 ;i < array.length ;i ++ )
			v2.add(array[i]);

		return v2;	
    }

    
    /**
     * 
     * @param mainString
     * @param inString
     * @return
     */
    public static boolean hasString(String mainString,String inString)
    {
    	int n = mainString.indexOf(inString);
    	if( n==-1) 
    		return false;
    	
    	return true;
    }

	/**
	 * 
	 * @param mainString
	 * @param inString
	 * @return
	 */
	public static String[] VectorToArray( Vector mainVector)
	{
		if( mainVector.size() == 0)
			return null;
		String[] v2 = new String[mainVector.size()];
		for( int i = 0; i < mainVector.size(); i++)
		{
			v2[i] = (String) mainVector.get( i);
		}
		return v2;
	}
	/**
	 * Check if the string is null or empty.
	 */
	public static boolean isEmpty( String input)
	{
		return (input == null || input.length() == 0);
	}
	/**
	 * Get the file name in the given file path name. Example input: D:\\A.txt
	 * D:A.txt D:/A.txt A.txt All the above's output is: A.txt
	 */
	public static String getFileName( String filePath)
	{
		if( filePath == null)
			return null;
		// Convert other OS's file separator to current OS's
		if( filePath.indexOf( '/') != -1)
		{
			filePath = filePath.replace( '/', File.separatorChar);
		}
		if( filePath.indexOf( '\\') != -1)
		{
			filePath = filePath.replace( '\\', File.separatorChar);
		}
		// Convert the driver separator to separator
		if( filePath.indexOf( ':') != -1)
		{
			filePath = filePath.replace( ':', File.separatorChar);
		}
		// Get the file name
		if( filePath.indexOf( File.separator) != -1)
		{
			filePath = filePath.substring( filePath
					.lastIndexOf( File.separator) + 1, filePath.length());
		}
		return filePath;
	}
	/**
	 * Get the directory name in the given file path name. Example input:
	 * D:\\A.txt D:A.txt D:/A.txt A.txt All the above's output is: A.txt
	 */
	public static String getDirName( String filePath)
	{
		if( filePath == null)
			return null;
		// Get the directory name
		if( filePath.indexOf( File.separator) != -1)
		{
			filePath = filePath.substring( 0, filePath
					.lastIndexOf( File.separator) + 1);
		}
		else if( filePath.indexOf( '/') != -1)
		{
			filePath = filePath.substring( 0, filePath.lastIndexOf( '/') + 1);
		}
		else if( filePath.indexOf( '\\') != -1)
		{
			filePath = filePath.substring( 0, filePath.lastIndexOf( '\\') + 1);
		}
		else if( filePath.indexOf( ':') != -1)
		{
			filePath = filePath.substring( 0, filePath.lastIndexOf( ':') + 1);
		}
		else
		{
			return null;
		}
		return filePath;
	}
	public static String getEntryName( String baseDir, String filePath)
	{
		if( StringUtil.isEmpty( baseDir))
			return getFileName( filePath);
		try
		{
			File base = new File( baseDir);
			File temp = new File( filePath);
			String baseName = base.getCanonicalPath().replace(
					File.separatorChar, '/');
			String fileName = temp.getCanonicalPath().replace(
					File.separatorChar, '/');
			// Because sometimes the OS doesn't retured a directory name
			// with file separator, so we fix it
			if( temp.isDirectory())
			{
				if( !fileName.endsWith( "/"))
				{
					fileName += "/";
				}
			}
			// If baseName is c:/folder or c:/folder/, then entryDirName is
			// folder or folder/
			String entryDirName = baseName.substring( baseName.lastIndexOf(
					'/', baseName.length() - 2) + 1);
			// If fileName is c:/folder/a.txt(c:/folder/sub/a.txt),
			// then entryFileName is a.txt(sub/a.txt)
			String entryFileName = fileName.substring( baseName.length());
			return entryDirName + entryFileName;
		}
		catch( Exception ex)
		{
			ex.printStackTrace();
			return "";
		}
	}
	/**
	 * Return the extension portion of the file's name .
	 * 
	 * @see #getExtension
	 */
	public static String getExtension( String fileName)
	{
		if( fileName != null)
		{
			int i = fileName.lastIndexOf( '.');
			if( i > 0 && i < fileName.length() - 1){ return fileName.substring(
					i + 1).toLowerCase(); }
		}
		return null;
	}
	public static boolean isDirectory( String filePath)
	{
		if( filePath == null)
			return false;
		// Convert other OS's file separator to current OS's
		if( filePath.indexOf( '/') != -1)
		{
			filePath = filePath.replace( '/', File.separatorChar);
		}
		if( filePath.indexOf( '\\') != -1)
		{
			filePath = filePath.replace( '\\', File.separatorChar);
		}
		// Convert the driver separator to separator
		if( filePath.indexOf( ':') != -1)
		{
			filePath = filePath.replace( ':', File.separatorChar);
		}
		return filePath.endsWith( File.separator);
	}
	/**
	 * 验证日期（待）
	 * @param text
	 * @param format
	 * @return
	 */
	public static boolean isDate(String text,String format){
		boolean isdate = false;
		try{
			SimpleDateFormat sf =new SimpleDateFormat( format );
			Date date = sf.parse( text );
			int dateyear = date.getYear()+1900;
			int datemonth = date.getMonth()+1;
			int datedate = date.getDate();
			int datehouse = date.getHours();
			
			String strdate = dateyear +"-";
			if (datemonth < 10 ) strdate = strdate +"0"+datemonth;
			else strdate = strdate +datemonth;
			if (datedate < 10 ) strdate = strdate +"-0"+datedate;
			else strdate = strdate +"-"+datedate;
			if (datehouse < 10 ) strdate = strdate +"-0"+datehouse;
			else strdate = strdate +"-"+datehouse;
			
			if (strdate.equalsIgnoreCase(text)) isdate = true;
			
			if ( dateyear > 3000 ) isdate = false;
			
		}catch(Exception ex){
			//isdate = false;
		}
		return isdate;
		
		
	}
	public static void main( String[] args)
	{
	}
}