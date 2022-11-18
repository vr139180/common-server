package com.cms.center.mybits.mapper;

import com.cms.center.mybits.entity.*;

import java.util.List;
import java.util.Map;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
public interface UserMapper
{
	int createUserAccount( User_Account record);
	User_Account getUserAccount( String acount);
}
