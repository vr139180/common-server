package com.cms.center.config;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializeConfig;
import com.alibaba.fastjson.serializer.SerializerFeature;
import com.alibaba.fastjson.serializer.ToStringSerializer;
import com.alibaba.fastjson.support.config.FastJsonConfig;
import com.alibaba.fastjson.support.spring.FastJsonHttpMessageConverter;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.converter.HttpMessageConverter;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurationSupport;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by lujf1978@163.com on 2020/10/30.
 * 解决Long类型序列化到json出现精度丢失问题
 * @version 1.00
 */
//@Configuration
public class FastJsonWebConfig extends WebMvcConfigurationSupport
{
	@Override
	protected void configureMessageConverters(List<HttpMessageConverter<?>> converters)
	{
		super.configureMessageConverters( converters);

		FastJsonHttpMessageConverter fastConverter = new FastJsonHttpMessageConverter();

		FastJsonConfig fastJsonConfig = new FastJsonConfig();

		SerializeConfig serializeConfig = SerializeConfig.globalInstance;
		serializeConfig.put(BigInteger.class, ToStringSerializer.instance);
		serializeConfig.put(Long.class, ToStringSerializer.instance);
		serializeConfig.put(Long.TYPE, ToStringSerializer.instance);

		fastJsonConfig.setSerializeConfig(serializeConfig);

		//关闭引用检测
		SerializerFeature[] fls = fastJsonConfig.getSerializerFeatures();
		List<SerializerFeature> nfls = new ArrayList<>();
		for( int ii =0; ii < fls.length; ++ii)
			nfls.add( fls[ii]);

		nfls.add( SerializerFeature.DisableCircularReferenceDetect);
		fls = nfls.toArray( new SerializerFeature[nfls.size()]);

		fastJsonConfig.setSerializerFeatures(fls);

		fastConverter.setFastJsonConfig( fastJsonConfig);

		converters.add(fastConverter);

		//关闭引用检测
		//JSON.DEFAULT_GENERATE_FEATURE |= SerializerFeature.DisableCircularReferenceDetect.getMask();
	}

	/**
	 * 解决 继承WebMvcConfigurationSupport之后swagger2 swagger-ui.html无法访问问题
	 * @param registry
	 */
	@Override
	public void addResourceHandlers(ResourceHandlerRegistry registry)
	{
		registry.addResourceHandler("/**").addResourceLocations(
				"classpath:/static/");
		registry.addResourceHandler("swagger-ui.html").addResourceLocations(
				"classpath:/META-INF/resources/");
		registry.addResourceHandler("/webjars/**").addResourceLocations(
				"classpath:/META-INF/resources/webjars/");
		super.addResourceHandlers(registry);
	}
}
