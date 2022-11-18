package com.cms.center.config;

import javafx.application.Platform;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;
import org.springframework.transaction.annotation.TransactionManagementConfigurer;

import javax.annotation.Resource;
import javax.sql.DataSource;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
@Configuration
@EnableTransactionManagement
public class MybitsTransactionConfig implements TransactionManagementConfigurer
{
	@Resource(name="txManagerJDBC")
	private PlatformTransactionManager txManagerDef;

	//jdbc platform
	@Bean( name = "txManagerJDBC")
	public PlatformTransactionManager txManager( DataSource dataSource)
	{
		return new DataSourceTransactionManager( dataSource);
	}

	//jpa platform
	/*
	@Bean( name = "txManagerJPA")
	public PlatformTransactionManager txManager2( EntityManagerFactory factory)
	{
		return new JpaTransactionManager( factory);
	}
	*/

	@Override
	public PlatformTransactionManager annotationDrivenTransactionManager()
	{
		return txManagerDef;
	}
}
