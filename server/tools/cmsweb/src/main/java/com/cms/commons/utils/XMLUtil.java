package com.cms.commons.utils;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
/**
 * Created by lujf1978@163.com on 2020/10/21.
 *
 * @version 1.00
 */
public class XMLUtil
{
	public static Map<String,Object> getMapFromXML(String xmlString)
			throws ParserConfigurationException, IOException, SAXException
	{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		InputStream is =  getStringStream(xmlString);
		Document document = builder.parse(is);

		//获取到document里面的全部结点
		NodeList allNodes = document.getFirstChild().getChildNodes();
		Node node;
		Map<String, Object> map = new HashMap<>();

		int i=0;
		while (i < allNodes.getLength())
		{
			node = allNodes.item(i);
			if(node instanceof Element)
				map.put(node.getNodeName(),node.getTextContent());

			i++;
		}

		return map;
	}

	protected static InputStream getStringStream( String sInputString)
	{
		ByteArrayInputStream tInputStringStream = null;
		if (sInputString != null && !sInputString.trim().equals(""))
		{
			tInputStringStream = new ByteArrayInputStream(sInputString.getBytes());
		}

		return tInputStringStream;
	}
}
