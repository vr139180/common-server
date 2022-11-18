package com.cms.commons.utils;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.Security;
import java.util.Arrays;

/**
 * Created by lujf1978@163.com on 2021/8/11.
 *
 * @version 1.00
 */
public class Pkcs7Encoder
{
	// 算法名称
	static  final String KEY_ALGORITHM = "AES";
	// 加解密算法/模式/填充方式
	static  final String algorithmStr = "AES/CBC/PKCS7Padding";
	private static Key key;
	private static Cipher cipher;
	boolean isInited = false;


	public static void init(byte[] keyBytes) {

		// 如果密钥不足16位，那么就补足.  这个if 中的内容很重要
		int base = 16;
		if (keyBytes.length % base != 0) {
			int groups = keyBytes.length / base + (keyBytes.length % base != 0 ? 1 : 0);
			byte[] temp = new byte[groups * base];
			Arrays.fill(temp, (byte) 0);
			System.arraycopy(keyBytes, 0, temp, 0, keyBytes.length);
			keyBytes = temp;
		}
		// 初始化
		Security.addProvider(new BouncyCastleProvider());
		// 转化成JAVA的密钥格式
		key = new SecretKeySpec(keyBytes, KEY_ALGORITHM);
		try {
			// 初始化cipher
			cipher = Cipher.getInstance(algorithmStr, "BC");
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchProviderException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}



	/**
	 * 加密方法
	 *      ---自定义对称解密算法初始向量 iv
	 * @param content
	 *              要加密的字符串
	 * @param keyBytes
	 *              加密密钥
	 * @param ivs
	 *         自定义对称解密算法初始向量 iv
	 * @return 加密的结果
	 */
	public static byte[] encryptOfDiyIV(byte[] content, byte[] keyBytes, byte[] ivs) {
		byte[] encryptedText = null;
		init(keyBytes);
		try {
			cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(ivs));
			encryptedText = cipher.doFinal(content);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return encryptedText;
	}
	/**
	 * 解密方法
	 *
	 * @param encryptedData
	 *            要解密的字符串
	 * @param keyBytes
	 *            解密密钥
	 * @param ivs
	 *         自定义对称解密算法初始向量 iv
	 * @return
	 */
	public static byte[] decryptOfDiyIV(byte[] encryptedData, byte[] keyBytes,byte[] ivs) throws Exception{
		byte[] encryptedText = null;
		init(keyBytes);
		cipher.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(ivs));
		encryptedText = cipher.doFinal(encryptedData);
		return encryptedText;
	}
}
