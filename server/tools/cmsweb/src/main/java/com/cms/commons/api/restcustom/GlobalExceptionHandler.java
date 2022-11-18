package com.cms.commons.api.restcustom;

import com.cms.commons.api.APIResultCode;
import com.cms.commons.api.exception.*;
import com.cms.commons.api.APICallResult;
import org.springframework.validation.FieldError;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import javax.validation.ValidationException;
import java.util.List;
import java.util.Set;

/**
 * Created by lujf1978@163.com on 2020/10/14.
 * 全局异常捕获
 * @version 1.00
 */
@ControllerAdvice
public class GlobalExceptionHandler
{
	/**
	 * 默认异常处理
	 * @param ex
	 * @return
	 */
	@ResponseBody
	@ExceptionHandler( value = Exception.class)
	public Object ExceptionHandler( Exception ex) throws Exception
	{
		APICallResult<Object> ret = new APICallResult<>(APIResultCode.SYSTEM_ERROR);
		ret.setMessage( ex.getMessage());

		return ret;
	}

	/**
	 * 数据库异常
	 * @param ex
	 * @return
	 */
	@ResponseBody
	@ExceptionHandler( value = CustomResultException.class)
	public Object customResultExceptionHandler( CustomResultException ex)
	{
		APICallResult<?> result = ex.getResult();

		if( result == null)
		{
			APICallResult<Object> ret = new APICallResult<>(APIResultCode.SYSTEM_ERROR);
			ret.setMessage( ex.getMessage());

			return ret;
		}
		else
		{
			return result;
		}
	}

	/**
	 * 数据库异常
	 * @param ex
	 * @return
	 */
	@ResponseBody
	@ExceptionHandler( value = DatabaseException.class)
	public Object databaseExceptionHandler( DatabaseException ex)
	{
		APICallResult<Object> ret = new APICallResult<>(APIResultCode.DATABASE_ERROR);
		ret.setMessage( ex.getMessage());

		return ret;
	}

	@ResponseBody
	@ExceptionHandler( value = RedisException.class)
	public Object redisExceptionHandler( RedisException ex)
	{
		APICallResult<Object> ret = new APICallResult<>(APIResultCode.REDIS_ERROR);
		ret.setMessage( ex.getMessage());

		return ret;
	}

	/**
	 * 参数校验异常
	 * @param ex
	 * @return
	 */
	@ResponseBody
	@ExceptionHandler( value = ValidationException.class)
	public Object validationExceptionHandler( ValidationException ex)
	{
		StringBuffer sb = new StringBuffer();
		if( ex instanceof ConstraintViolationException)
		{
			ConstraintViolationException ex1 = (ConstraintViolationException)ex;
			Set<ConstraintViolation<?>> violations = ex1.getConstraintViolations();
			for( ConstraintViolation<?> item : violations)
			{
				sb.append(item.getMessage());
				sb.append( "\r\n");
			}
		}

		APICallResult<Object> ret = new APICallResult<>(APIResultCode.PARAM_ERROR);
		ret.setMessage( sb.toString());

		return ret;
	}

	@ResponseBody
	@ExceptionHandler( value = ValidateParamException.class)
	public Object valideParamExceptionHandler( ValidateParamException ex)
	{
		StringBuffer sb = new StringBuffer();
		List<String> errorInfo = ex.getErrors();
		if( errorInfo != null)
		{
			for( String item:errorInfo)
			{
				sb.append( item);
				sb.append("\r\n");
			}
		}

		APICallResult<Object> ret = new APICallResult<>(APIResultCode.PARAM_ERROR);
		ret.setMessage( sb.toString());

		return ret;
	}

	@ResponseBody
	@ExceptionHandler( value = MethodArgumentNotValidException.class)
	public Object methArgumentNotValidHandler( MethodArgumentNotValidException ex)
	{
		StringBuffer sb = new StringBuffer();
		for(FieldError error : ex.getBindingResult().getFieldErrors())
		{
			sb.append(error.getField() + ":" + error.getDefaultMessage());
			sb.append( "\r\n");
		}

		APICallResult<Object> ret = new APICallResult<>(APIResultCode.PARAM_ERROR);
		ret.setMessage( sb.toString());

		return ret;
	}

}
