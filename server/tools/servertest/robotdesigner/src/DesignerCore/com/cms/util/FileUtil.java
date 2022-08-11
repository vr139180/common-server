package com.cms.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

/**
 * @author Guan AiJun 2004.6 $Id: FileUtil.java,v 1.1.2.1 2004/06/08 02:57:10
 *         gaj Exp $
 */
public class FileUtil
{
	public static void deleteFile( File f)
	{
		if( f.isDirectory())
		{
			File d[] = f.listFiles();
			for( int i = 0; i < d.length; ++i)
				deleteFile( d[i]);
		}
		f.delete();
	}

	public static boolean copy( File input, File output)
	{
		int BUFSIZE = 2154;
		try
		{
			FileInputStream fis = new FileInputStream( input);
			FileOutputStream fos = new FileOutputStream( output);

			int s;
			byte[] buf = new byte[BUFSIZE];
			while( (s = fis.read( buf)) > -1)
			{
				fos.write( buf, 0, s);
			}
			fis.close();
			fos.close();
		}
		catch( Exception ex)
		{
			ex.printStackTrace();
		}
		return true;
	}

	/**
	 * This class copies an input files of a directory to another directory
	 * include subdir
	 */
	public static void copyDir( File source, File dest)
	{
		String[] files = null;
		int i = 0;
		File sourcefile = null;
		File destfile = null;

		try
		{
			if( source.isFile())
			{
				if( dest.isFile())
					copy( source, dest);
				else
				{
					destfile =new File( dest.getAbsolutePath() + File.separator + source.getName());
					if( !destfile.exists())
						destfile.createNewFile();
					copy( source, destfile);
				}
				return;
			}
			else
			{
				dest.mkdirs();
				files = source.list();
				for( i = 0; i < files.length; i++)
				{
					sourcefile = new File( source + File.separator + files[i]);
					destfile = new File( dest + File.separator + files[i]);
					if( sourcefile.isDirectory())
						destfile.mkdirs();
					else
						destfile.createNewFile();
					copyDir( sourcefile, destfile);
				}
			}
		}
		catch( Exception e)
		{
		}
	}
}
