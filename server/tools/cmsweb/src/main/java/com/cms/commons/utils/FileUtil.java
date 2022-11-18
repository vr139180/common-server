package com.cms.commons.utils;

/**
 * Created by lujf1978@163.com on 2020/10/18.
 *
 * @version 1.00
 */
public class FileUtil
{
	/**
	 * 获取文件的扩展名
	 * @param file
	 * @return
	 */
	public static String getFileExt( String file)
	{
		if( file == null || file.length() == 0)
			return "";

		int ind = file.lastIndexOf( ".");
		if( ind == -1)
			return "";
		return file.substring( ind + 1);
	}
}
