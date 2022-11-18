package com.cms.center.services;

import com.cms.center.controller.resp.GetUserAccountRTO;
import com.cms.commons.api.APICallResult;


/**
 * Created by lujf1978@163.com on 2020/8/18.
 */
public interface UserHomeService
{
	APICallResult<GetUserAccountRTO> getUserAccount( String account);
}
