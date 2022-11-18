package com.cms.commons.utils;

import java.security.MessageDigest;

/**
 * Created by lujf1978@163.com on 2021/9/24.
 *
 * @version 1.00
 */
public class SHA1Util
{
	public static String sha( String str)
	{
		try{
			MessageDigest sha = MessageDigest.getInstance("SHA");
			byte[] byteArray = str.getBytes( "UTF-8");
			byte[] md = sha.digest( byteArray);
			StringBuffer sb = new StringBuffer();
			for( int i =0; i < md.length; ++i)
			{
				int val = ((int)md[i]) & 0xff;
				if( val < 16)
					sb.append( "0");
				sb.append( Integer.toHexString( val));
			}

			return sb.toString();
		}
		catch(Exception e){
			return "";
		}
	}
}
