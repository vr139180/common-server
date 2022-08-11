package com.cms.core.workflow.parser.dom4j;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.dom4j.QName;
import org.dom4j.Element;

import com.cms.core.workflow.parser.XPDLParserException;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: Util.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class Util
{
	// perhaps this should be configurable?
	private static final DateFormat STANDARD_DF = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss");

	private Util()
	{
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

	public static Date elementAsDate( Element element, String name) throws XPDLParserException
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

}
