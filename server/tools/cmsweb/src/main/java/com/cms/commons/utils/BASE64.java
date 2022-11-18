package com.cms.commons.utils;

import org.apache.commons.codec.binary.Base64;

/**
 * Created by lujf1978@163.com on 2021/8/11.
 *
 * @version 1.00
 */
public class BASE64
{
	/** * BASE64解密 * @param key * @return * @throws Exception */
	public static byte[] decryptBASE64(byte[] key){
		return (new Base64()).decode(key);
	}

	/** * BASE64加密 * @param key * @return * @throws Exception */
	public static byte[] encryptBASE64(byte[] key){
		return (new Base64()).encode(key);
	}
}
