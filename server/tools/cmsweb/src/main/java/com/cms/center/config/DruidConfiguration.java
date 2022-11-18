package com.cms.center.config;

import com.alibaba.druid.pool.DruidDataSource;
import com.alibaba.druid.spring.boot.autoconfigure.DruidDataSourceBuilder;
import com.alibaba.druid.support.http.StatViewServlet;
import com.alibaba.druid.support.http.WebStatFilter;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.boot.web.servlet.ServletRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;

import javax.sql.DataSource;
import java.sql.SQLException;
import java.util.List;

/**
 * Created by lujf1978@163.com on 2020/8/19.
 *
 * @version 1.00
 */
@Configuration
@EnableAutoConfiguration
public class DruidConfiguration
{
	@Value("${spring.datasource.url}")
	private String dbUrl;
	@Value("${spring.datasource.username}")
	private String username;
	@Value("${spring.datasource.password}")
	private String password;
	@Value("${spring.datasource.driver-class-name}")
	private String driverClassName;
	@Value("${spring.datasource.initialSize}")
	private int initialSize;
	@Value("${spring.datasource.minIdle}")
	private int minIdle;
	@Value("${spring.datasource.maxActive}")
	private int maxActive;
	@Value("${spring.datasource.maxWait}")
	private int maxWait;
	@Value("${spring.datasource.timeBetweenEvictionRunsMillis}")
	private int timeBetweenEvictionRunsMillis;
	@Value("${spring.datasource.minEvictableIdleTimeMillis}")
	private int minEvictableIdleTimeMillis;
	@Value("${spring.datasource.validationQuery}")
	private String validationQuery;
	@Value("${spring.datasource.testWhileIdle}")
	private boolean testWhileIdle;
	@Value("${spring.datasource.testOnBorrow}")
	private boolean testOnBorrow;
	@Value("${spring.datasource.testOnReturn}")
	private boolean testOnReturn;
	@Value("${spring.datasource.poolPreparedStatements}")
	private boolean poolPreparedStatements;
	@Value("${spring.datasource.filters}")
	private String filters;
	@Value("${spring.datasource.connectionProperties}")
	private String connectionProperties;
	@Value("${spring.datasource.useGlobalDataSourceStat}")
	private boolean useGlobalDataSourceStat;
	@Value("${spring.datasource.connection-init-sqls}")
	private List<String> connectionInitSqls;

	@Bean
	public ServletRegistrationBean druidServlet()
	{
		ServletRegistrationBean reg = new ServletRegistrationBean();
		reg.setServlet(new StatViewServlet());
		reg.addUrlMappings("/druid/*");

		// IP白名单
		//reg.addInitParameter("allow", "192.168.2.25,127.0.0.1");
		// IP黑名单(共同存在时，deny优先于allow)
		//reg.addInitParameter("deny", "192.168.1.100");

		//控制台管理用户
		reg.addInitParameter("loginUsername", "admin");
		reg.addInitParameter("loginPassword", "123456");
		//是否能够重置数据 禁用HTML页面上的“Reset All”功能
		reg.addInitParameter("resetEnable", "false");

		return reg;
	}

	@Bean
	public FilterRegistrationBean filterRegistrationBean()
	{
		FilterRegistrationBean filterRegistrationBean = new FilterRegistrationBean();

		filterRegistrationBean.setFilter(new WebStatFilter());

		filterRegistrationBean.addUrlPatterns("/*");
		filterRegistrationBean.addInitParameter("exclusions", "*.js,*.gif,*.jpg,*.png,*.css,*.ico,/druid/*");

		return filterRegistrationBean;
	}

	@Bean
	@Primary
	public DataSource dataSource()
	{
		DruidDataSource datasource = new DruidDataSource();

		datasource.setUrl(this.dbUrl);
		datasource.setUsername(username);
		datasource.setPassword(password);
		datasource.setDriverClassName(driverClassName);

		datasource.setInitialSize(initialSize);
		datasource.setMinIdle(minIdle);
		datasource.setMaxActive(maxActive);
		datasource.setMaxWait(maxWait);
		datasource.setTimeBetweenEvictionRunsMillis(timeBetweenEvictionRunsMillis);

		datasource.setMinEvictableIdleTimeMillis(minEvictableIdleTimeMillis);
		datasource.setValidationQuery(validationQuery);

		datasource.setTestWhileIdle(testWhileIdle);
		datasource.setTestOnBorrow(testOnBorrow);
		datasource.setTestOnReturn(testOnReturn);

		datasource.setPoolPreparedStatements(poolPreparedStatements);
		datasource.setConnectionInitSqls(connectionInitSqls);

		try
		{
			datasource.setFilters(filters);
		}
		catch (SQLException e)
		{
			e.printStackTrace();
		}

		datasource.setConnectionProperties(connectionProperties);
		datasource.setUseGlobalDataSourceStat(useGlobalDataSourceStat);

		return datasource;
	}
}
