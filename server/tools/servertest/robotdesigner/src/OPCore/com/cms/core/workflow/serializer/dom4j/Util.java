package com.cms.core.workflow.serializer.dom4j;

import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.dom4j.QName;
import org.dom4j.Element;

/**
 * Dom4J XPDL Serializer utility class.
 */
class Util
{
	private static final DateFormat STANDARD_DF = new SimpleDateFormat( "yyyy-MM-dd HH:mm:ss");

	private Util()
	{
		// no op
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


}
