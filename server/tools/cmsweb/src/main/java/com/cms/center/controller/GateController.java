package com.cms.center.controller;

import com.cms.center.controller.resp.EurekaMasterRTO;
import com.cms.center.services.EurekaService;
import com.cms.commons.api.APICallResult;
import com.cms.commons.api.APIResultCode;
import com.cms.commons.api.exception.RedisException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;


/**
 * Created by lujf1978@163.com on 2020/8/18.
 *
 * @version 1.00
 */
@RestController
@RequestMapping("/gate")
public class GateController
{
	private static final Logger logger = LogManager.getLogger(GateController.class);

	@Autowired
	private EurekaService eurekaService;

	@RequestMapping( value="/eureka", method = RequestMethod.GET)
	@ResponseBody
	public APICallResult<EurekaMasterRTO> eureka()
	{
		EurekaMasterRTO nod = eurekaService.getEurekaMasterNode();
		if( nod == null)
			throw new RedisException();

		return new APICallResult<>(APIResultCode.SUCCESS, nod);
	}
}
