package com.cms.center;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.data.mongo.MongoDataAutoConfiguration;
import org.springframework.boot.autoconfigure.mongo.MongoAutoConfiguration;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.ComponentScans;

@SpringBootApplication()
@EnableAutoConfiguration(exclude = {MongoAutoConfiguration.class, MongoDataAutoConfiguration.class})
@MapperScan(basePackages = "com.cms.center.mybits.mapper")
@ComponentScans({
		@ComponentScan("com.cms.commons.redis"),
		@ComponentScan("com.cms.commons.api.restcustom"),
		@ComponentScan("com.cms.center")
})
public class CMSWebApplication
{

	public static void main(String[] args) {

		SpringApplication.run(CMSWebApplication.class, args);
	}

}
