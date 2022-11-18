package com.cms.center.services.impl;

import com.cms.center.controller.resp.GetUserAccountRTO;
import com.cms.center.mybits.entity.*;
import com.cms.center.mybits.mapper.UserMapper;
import com.cms.commons.api.APIResultCode;
import com.cms.commons.utils.MD5Util;
import com.cms.commons.api.APICallResult;
import com.cms.center.services.UserHomeService;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * Created by lujf1978@163.com on 2020/8/18.
 */
@Service
public class UserHomeServiceImpl implements UserHomeService
{
	private static final Logger logger = LogManager.getLogger( UserHomeServiceImpl.class);

	@Autowired
	private UserMapper userMapper;

	@Override
	public APICallResult<GetUserAccountRTO> getUserAccount(String account)
	{
		account = account.toLowerCase();

		User_Account user = userMapper.getUserAccount(account);
		if( user == null)
		{
			user = new User_Account();
			user.setAccount( account);
			user.setPwd(MD5Util.MD5Encode(""));
			user.setState( 0);

			userMapper.createUserAccount(user);
		}

		GetUserAccountRTO rto = new GetUserAccountRTO();
		rto.setUseriid( user.getUser_iid());
		rto.setGates( "127.0.0.1:8001");

		return new APICallResult<>( APIResultCode.SUCCESS, rto);
	}
}
