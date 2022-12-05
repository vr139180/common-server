package com.cms.center.controller;

import com.cms.center.controller.resp.GetUserAccountRTO;
import com.cms.center.mybits.entity.User_Account;
import com.cms.center.redis.GateNodeInfo;
import com.cms.center.services.EurekaService;
import com.cms.center.services.UserHomeService;
import com.cms.commons.api.APICallResult;
import com.cms.commons.api.APIResultCode;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;
import java.util.List;

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
	@Autowired
	private EurekaService	eurekaSvr;

	@RequestMapping( value="/get", method = RequestMethod.GET)
	@ResponseBody
	public APICallResult<GetUserAccountRTO> getAccount(@Valid @RequestParam(value = "account") String account,
													   @Valid @RequestParam(value = "type") Integer type)
	{
		User_Account acc = homeService.getUserAccount( account);
		if( type == null || type < 1 || type > 2)
			type = 2;
		List<GateNodeInfo> gates = eurekaSvr.getGateNodeList(type);
		StringBuffer sb = new StringBuffer();
		for( GateNodeInfo ni : gates)
		{
			if( sb.length() > 0)
				sb.append( ",");
			sb.append( ni.getIp());
			sb.append( ":");
			sb.append( ni.getPort());
		}
		GetUserAccountRTO rto = new GetUserAccountRTO();
		rto.setUseriid( acc.getUser_iid());
		rto.setGates( sb.toString());

		return new APICallResult<>(APIResultCode.SUCCESS, rto);
	}
}
