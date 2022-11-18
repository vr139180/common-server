package com.cms.commons.utils;

import java.util.Random;

/**
 * Created by lujf1978@163.com on 2020/10/18.
 *
 * @version 1.00
 */
public class RandomUtil
{
	/**
	 * 获取4位验证码
	 * @return
	 */
	public static String randomVerifyCode()
	{
		int max=9999;
		int min=999;
		Random random = new Random();

		int s = random.nextInt(max)%(max-min+1) + min;
		return String.format( "%04d", s);
	}
}
