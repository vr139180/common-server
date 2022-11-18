package com.cms.commons.utils;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Created by Administrator on 2020/8/18.
 */
public class InputStreamUtil
{
	public static int BUFFER_SIZE=4096;

	/**
	 * 将一个InputStream的数据读到数组中。
	 * (该方法不关闭InputStream)
	 *
	 * @param is
	 * @return
	 */
	public static byte[] readInputStream(InputStream is) throws IOException
	{
		ByteArrayOutputStream baos=new ByteArrayOutputStream();
		byte[] buffer=new byte[BUFFER_SIZE];
		int len;
		while((len=is.read(buffer))!=-1)
		{
			baos.write(buffer,0,len);
		}

		return baos.toByteArray();
	}

	/**
	 * 将输入流的内容复制到输出流。
	 * (该方法将关闭InputStream和OutputStream)
	 *
	 * @param is
	 * @param os
	 * @throws IOException
	 */
	public static void copyStream(InputStream is,OutputStream os) throws IOException
	{
		try
		{
			byte[] buffer=new byte[BUFFER_SIZE];
			int len;
			while((len=is.read(buffer))!=-1)
			{
				os.write(buffer,0,len);
			}
		}
		finally
		{
			try{is.close();}catch(IOException e){}
			try{os.close();}catch(IOException e){}
		}
	}

}
