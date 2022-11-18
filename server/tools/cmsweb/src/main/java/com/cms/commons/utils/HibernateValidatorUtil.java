package com.cms.commons.utils;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import javax.validation.Validation;
import javax.validation.Validator;
import java.util.Set;

/**
 * Created by lujf1978@163.com on 2020/10/18.
 *
 * @version 1.00
 */
public class HibernateValidatorUtil
{
	private static Validator validator;

	static {
		validator = Validation.buildDefaultValidatorFactory().getValidator();
	}

	public static void validateEntity(Object object, Class<?>... groups)
	{
		// 用验证器执行验证，返回一个违反约束的set集合
		Set<ConstraintViolation<Object>> violationSet = validator.validate(object, groups);
		// 判断是否为空，空：说明验证通过，否则就验证失败
		if(!violationSet.isEmpty())
		{
			throw new ConstraintViolationException( violationSet);
		}

	}
}
