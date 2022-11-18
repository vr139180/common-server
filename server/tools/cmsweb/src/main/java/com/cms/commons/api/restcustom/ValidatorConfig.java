package com.cms.commons.api.restcustom;

import org.hibernate.validator.HibernateValidator;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.validation.beanvalidation.MethodValidationPostProcessor;

import javax.validation.Validation;
import javax.validation.Validator;
import javax.validation.ValidatorFactory;

/**
 * Created by lujf1978@163.com on 2020/10/14.
 * 参数校验配置
 * @version 1.00
 */
@Configuration
@EnableAutoConfiguration
public class ValidatorConfig
{
	@Bean
	public MethodValidationPostProcessor methodValidationPostProcessor()
	{
		MethodValidationPostProcessor postProcessor = new MethodValidationPostProcessor();
		postProcessor.setValidator(validator());
		return postProcessor;
	}

	/**
	 * 使用快速校验模式，一旦出错立刻返回
	 * @return
	 */
	@Bean
	public Validator validator()
	{
		ValidatorFactory validatorFactory = Validation.byProvider(HibernateValidator.class)
				.configure()
				.addProperty("hibernate.validator.fail_fast", "true")
				.buildValidatorFactory();
		Validator validator = validatorFactory.getValidator();
		return validator;
	}
}
