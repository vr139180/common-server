package com.cms.center.controller;

import com.cms.center.controller.resp.GetUserAccountRTO;
import com.cms.center.services.UserHomeService;
import com.cms.commons.api.APICallResult;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;

/**
 * Created by lujf1978@163.com on 2020/8/18.
 *
 * @version 1.00
 */
@RestController
@RequestMapping("/account")
public class UserAccountController
{
	private static final Logger logger = LogManager.getLogger(UserAccountController.class);

	@Autowired
	private UserHomeService homeService;

	@RequestMapping( value="/get", method = RequestMethod.GET)
	@ResponseBody
	public APICallResult<GetUserAccountRTO> getAccount(@Valid @RequestParam(value = "account") String account)
	{
		return homeService.getUserAccount( account);
	}
}
