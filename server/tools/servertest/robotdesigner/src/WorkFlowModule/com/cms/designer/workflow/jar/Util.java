package com.cms.designer.workflow.jar;

/**
 * @author today
 * 
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */

import java.net.URL;
import java.net.MalformedURLException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import java.io.*;

import org.dom4j.QName;
import org.dom4j.Element;
import org.dom4j.io.*;
import org.dom4j.*;

public class Util
{

	// 从文件读取XML，输入文件名，返回XML文档
	public static Document read( String context)
	throws MalformedURLException, DocumentException, IOException
	{
		SAXReader reader = new SAXReader();
		InputStream in =new StringBufferInputStream( context);
		Document document = reader.read( in);
		in.close();
		return document;
	}

	public static Document read( File file)
	throws MalformedURLException, DocumentException, IOException
	{
		SAXReader reader = new SAXReader();
		InputStream in =new FileInputStream( file);
		Document document = reader.read( in);
		in.close();
		return document;
	}

	//取得Root节点
	public static Element getRootElement( Document doc)
	{
		return doc.getRootElement();
	}

	/**
	 * Return the child element with the given name. The element must be in the
	 * same name space as the parent element.
	 * 
	 * @param element
	 *            The parent element
	 * @param name
	 *            The child element name
	 * @return The child element
	 */

	public static Element child( Element element, String name)
	{
		return element.element( new QName( name, element.getNamespace()));
	}

	/**
	 * Return the child elements with the given name. The elements must be in
	 * the same name space as the parent element.
	 * 
	 * @param element
	 *            The parent element
	 * @param name
	 *            The child element name
	 * @return The child elements
	 */

	public static List children( Element element, String name)
	{
		return element.elements( new QName( name, element.getNamespace()));
	}

	// Conversion

	/**
	 * Return the value of the child element with the given name. The element
	 * must be in the same name space as the parent element.
	 * 
	 * @param element
	 *            The parent element
	 * @param name
	 *            The child element name
	 * @return The child element value
	 */

	public static String elementAsString( Element element, String name)
	{
		return element.elementTextTrim( new QName( name, element.getNamespace()));
	}

	public static Date elementAsDate( Element element, String name)
	{
		String text = elementAsString( element, name);
		if( text == null){ return null; }

		try
		{
			return STANDARD_DF.parse( text);
		}
		catch( ParseException e)
		{
			return null;
			//throw new XPDLParserException("Error parsing date: " + text, e);
		}
	}

	public static int elementAsInteger( Element element, String name)
	{
		String text = elementAsString( element, name);
		if( text == null){ return 0; }

		return Integer.parseInt( text);
	}

	public static boolean elementAsBoolean( Element element, String name)
	{
		String text = elementAsString( element, name);
		if( text == null){ return false; }

		return new Boolean( text).booleanValue();
	}

	/**
	 * Add a child element with the specific name to the given parent element
	 * and return the child element. This method will use the namespace of the
	 * parent element for the child element's namespace.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @return The child element
	 */

	static Element addElement( Element parent, String name)
	{
		return parent.addElement( new QName( name, parent.getNamespace()));
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, Date value)
	{
		return addElement( parent, name, value, null);
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * If the given value is null then the default value is used. If the value
	 * is null then this method will not add the child element and will return
	 * null.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @param defaultValue
	 *            The default value (if the value is null)
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, Date value, Date defaultValue)
	{
		Element child = null;

		if( value == null)
		{
			value = defaultValue;
		}

		if( value != null)
		{
			child = addElement( parent, name);
			child.addText( STANDARD_DF.format( value));
		}

		return child;
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, String value)
	{
		return addElement( parent, name, value, null);
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * If the given value is null then the default value is used. If the value
	 * is null then this method will not add the child element and will return
	 * null.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @param defaultValue
	 *            The default value (if the value is null)
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, String value, String defaultValue)
	{
		Element child = null;

		if( value == null)
		{
			value = defaultValue;
		}

		if( value != null)
		{
			child = addElement( parent, name);
			child.addText( value);
		}

		return child;
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, URL value)
	{
		return addElement( parent, name, value, null);
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * If the given value is null then the default value is used. If the value
	 * is null then this method will not add the child element and will return
	 * null.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @param defaultValue
	 *            The default value (if the value is null)
	 * @return The child element
	 */

	static Element addElement( Element parent, String name, URL value, URL defaultValue)
	{
		Element child = null;

		if( value == null)
		{
			value = defaultValue;
		}

		if( value != null)
		{
			child = addElement( parent, name);
			child.addText( value.toString());
		}

		return child;
	}

	/**
	 * Add a child element with the specific name and the given value to the
	 * given parent element and return the child element. This method will use
	 * the namespace of the parent element for the child element's namespace.
	 * 
	 * @param parent
	 *            The parent element
	 * @param name
	 *            The new child element name
	 * @param value
	 *            The value
	 * @return The child element
	 */

	// perhaps this should be configurable?
	private static final DateFormat STANDARD_DF = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss");

}
