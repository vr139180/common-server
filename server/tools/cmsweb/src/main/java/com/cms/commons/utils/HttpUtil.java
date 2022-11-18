package com.cms.commons.utils;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.InputStreamEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Base64;
import java.util.HashMap;
import java.util.List;

/**
 * Created by lujf1978@163.com on 2021/8/11.
 *
 * @version 1.00
 */
public class HttpUtil
{
	public static final String DEFAULT_CHARSET = "UTF-8";

	public static String post(String url, HashMap<String, String> data, int timeout) throws Exception
	{
		return post(url,data,timeout,DEFAULT_CHARSET);
	}

	public static String post(String url, HashMap<String, String> data, int timeout, final String charset) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		String r = null;

		try
		{
			HttpPost post=new HttpPost(url);
			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				post.setConfig(requestConfig);
			}

			List<NameValuePair> params = new ArrayList<NameValuePair>();
			for(String name:data.keySet())
			{
				params.add(new BasicNameValuePair(name, data.get(name)));
			}

			System.out.println(new UrlEncodedFormEntity(params,DEFAULT_CHARSET).toString());
			post.setEntity(new UrlEncodedFormEntity(params,DEFAULT_CHARSET));

			r = httpclient.execute(post, new ResponseHandler<String>() {
				public String handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException {
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						return entity != null ? EntityUtils.toString(entity,charset) : null;
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});
		}
		finally
		{
			httpclient.close();
		}

		return r;
	}

	public static String get(String url, int timeout) throws Exception
	{
		return get(url,timeout,DEFAULT_CHARSET);
	}

	public static String get(String url, int timeout, final String charset) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		String r = null;

		try
		{
			HttpGet get=new HttpGet(url);

			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				get.setConfig(requestConfig);
			}

			r = httpclient.execute(get, new ResponseHandler<String>() {
				public String handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException {
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						return entity != null ? EntityUtils.toString(entity,charset) : null;
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});


		}
		finally
		{
			httpclient.close();
		}
		return r;
	}

	public static boolean get(String url, int timeout, final OutputStream output) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		boolean r;

		try
		{
			HttpGet get=new HttpGet(url);

			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				get.setConfig(requestConfig);
			}

			r = httpclient.execute(get, new ResponseHandler<Boolean>() {

				public Boolean handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException {
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						if(entity!=null)
						{
							entity.writeTo(output);
							return true;
						}
						else
						{
							return false;
						}
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});
		}
		finally
		{
			httpclient.close();
		}

		return r;
	}

	public static String post(String url, byte[] data, int timeout, final String charset) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		String r = null;

		try
		{
			HttpPost post=new HttpPost(url);
			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				post.setConfig(requestConfig);
			}

			post.setEntity(new InputStreamEntity(new ByteArrayInputStream(data)));

			r = httpclient.execute(post, new ResponseHandler<String>() {
				public String handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException {
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						return entity != null ? EntityUtils.toString(entity,charset) : null;
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});
		}
		finally
		{
			httpclient.close();
		}

		return r;
	}

	public static String post(String url, String data, int timeout, final String charset) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		String r = null;

		try
		{
			HttpPost post=new HttpPost(url);

			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				post.setConfig(requestConfig);
			}

			post.setEntity(new StringEntity(data,charset));

			r = httpclient.execute(post, new ResponseHandler<String>() {
				public String handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException
				{
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						return entity != null ? EntityUtils.toString(entity, charset) : null;
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});
		}
		finally
		{
			httpclient.close();
		}
		return r;
	}

	public static String postJson(String url, HashMap<String, Object> data, int timeout) throws Exception
	{
		CloseableHttpClient httpclient = HttpClients.createDefault();
		String r = null;

		try
		{
			HttpPost post=new HttpPost(url);

			if(timeout>0)
			{
				RequestConfig requestConfig = RequestConfig.copy(RequestConfig.DEFAULT)
						.setSocketTimeout(timeout)
						.setConnectTimeout(timeout)
						.setConnectionRequestTimeout(timeout).build();

				post.setConfig(requestConfig);
			}

			String strdata = JSONUtil.objToJsonStr( data);
			post.setEntity(new StringEntity( strdata, "UTF-8"));

			r = httpclient.execute(post, new ResponseHandler<String>() {
				public String handleResponse(HttpResponse response)
						throws ClientProtocolException, IOException
				{
					int status = response.getStatusLine().getStatusCode();
					if (status >= 200 && status < 300) {
						HttpEntity entity = response.getEntity();
						if( entity == null)
							return null;

						byte[] x = entity != null ? EntityUtils.toByteArray(entity) : null;
						String s = Base64.getEncoder().encodeToString( x);
						return s;
					} else {
						throw new ClientProtocolException("Unexpected response status: " + status);
					}
				}
			});
		}
		finally
		{
			httpclient.close();
		}
		return r;
	}
}
