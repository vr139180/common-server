package com.cms.core.workflow.parser.dom4j;

import java.io.InputStream;
import java.io.IOException;
import java.util.Map;
import java.util.List;
import java.util.Iterator;

import org.dom4j.Element;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.io.SAXReader;
import org.dom4j.tree.DefaultCDATA;

import com.cms.core.util.*;
import com.cms.core.util.log.LogManager;
import com.cms.core.util.log.Logger;
import com.cms.core.workflow.*;
import com.cms.core.workflow.activity.*;
import com.cms.core.workflow.application.*;
import com.cms.core.workflow.condition.*;
import com.cms.core.workflow.parser.ExtensionNames;
import com.cms.core.workflow.parser.XPDLParser;
import com.cms.core.workflow.parser.XPDLParserException;
import com.cms.core.workflow.transition.*;
/**
 * Implementation of the XPDLParser interface which uses Dom4J.
 */
public class Dom4JXPDLParser implements XPDLParser, ExtensionNames
{
	private static Logger log = LogManager.getLogger( Dom4JXPDLParser.class.getName());

	private InstantiationType defaultInstantiationType = InstantiationType.ONCE;

	/** Construct a new Dom4JXPDLParser. */
	public Dom4JXPDLParser()
	{
	}

	/**
	 * Parse the XPDL document which is provided by the given InputStream.
	 * 
	 * @param in
	 *            The XPDL stream
	 * @return The Workflow Package
	 * @throws IOException
	 *             Any I/O Exception
	 * @throws XPDLParserException
	 *             Any parser exception
	 */
	public WorkflowPackage parse( InputStream in) throws IOException, XPDLParserException
	{
		try
		{
			SAXReader reader = new SAXReader();
			Document document = reader.read( in);

			Element packageElement = document.getRootElement();

			// construct the Package
			WorkflowPackage pkg = new WorkflowPackage( packageElement.attributeValue( ID), packageElement.attributeValue( NAME));

			loadWorkflowProcesses( pkg.getWorkflowProcesses(), packageElement);

			loadExtendedAttributes( pkg.getExtendedAttributes(), Util.child( packageElement, EXTENDED_ATTRIBUTES));

			return pkg;
		}
		catch( DocumentException e)
		{
			e.printStackTrace();
			throw new XPDLParserException( "Error parsing document.", e);
			
		}
	
	}

	// WorkflowProcess related
	/**
	 * Load the workflow process objects from the given list of
	 * "WorkflowProcess" elements.
	 * 
	 * @param workflowProcesses
	 *            The list of workflow processes
	 * @param elements
	 *            The list of "WorkflowProcess" elements
	 * @throws XPDLParserException
	 */
	protected void loadWorkflowProcesses( List workflowProcesses, Element elements)
			throws XPDLParserException
	{
		workflowProcesses.clear();
		
		Element e =Util.child( elements, WORKFLOW_PROCESS);

		workflowProcesses.add( createWorkflowProcess( e));
	}

	protected WorkflowProcess createWorkflowProcess( Element element) throws XPDLParserException
	{
		WorkflowProcess wp = new BasicWorkflowProcess( element.getParent().attributeValue( ID), element.getParent().attributeValue( NAME));

		loadActivities( wp.getActivities(), element);

		loadTransitions( wp.getTransitions(), Util.child( element.getParent(), TRANSITIONS));

		loadWFExtendedAttributes( wp.getExtendedAttributes(), Util.child( element.getParent(),
				EXTENDED_ATTRIBUTES));

		//初始化计数值
		String tranid = (String) wp.getExtendedAttributes().get( "auto.transition.autoid");
		String actid = (String) wp.getExtendedAttributes().get( "auto.activity.autoid");
		
		wp.initTransitionId( tranid);
		wp.initActivityId( actid);

		return wp;
	}

	protected void loadWFExtendedAttributes( Map extendedAttributes, Element element)
			throws XPDLParserException
	{
		if( element == null)
			return;

		List elements = Util.children( element, EXTENDED_ATTRIBUTE);

		if( elements == null)
			return;
		extendedAttributes.clear();

		Iterator iter = elements.iterator();
		while( iter.hasNext())
		{
			Element extendedAttributeElement = (Element) iter.next();

			String n = extendedAttributeElement.attributeValue( NAME);
			String v = extendedAttributeElement.attributeValue( VALUE);
			extendedAttributes.put( n, v);
		} //while
	}

	// Activity Related
	/**
	 * Load actvities from the given Element. The element should represent a
	 * collection of activities (which is identified by the "Activities" tag).
	 * 
	 * @param activities
	 *            The list of activities
	 * @param element
	 *            The "Activities" element
	 * @throws XPDLParserException
	 */
	protected void loadActivities( List activities, Element element) throws XPDLParserException
	{
		if( element == null)
		{
			log.warn( "Activites element was null");
			return;
		}

		loadActivities( activities, Util.children( element, ACTIVITY));
	}

	/**
	 * Load the activity objects from the given list of "Activity" elements.
	 * 
	 * @param activites
	 *            The list of activites
	 * @param elements
	 *            The list of "Activity" elements
	 * @throws XPDLParserException
	 */
	protected void loadActivities( List activities, List elements) throws XPDLParserException
	{
		activities.clear();

		Iterator iter = elements.iterator();
		while( iter.hasNext())
		{
			Element activityElement = (Element) iter.next();
			activities.add( createActivity( activityElement));
		}
	}

	protected Activity createActivity( Element element) throws XPDLParserException
	{
		Activity activity = new Activity( element.attributeValue( ID), element.attributeValue( NAME));
		
		Element acts =Util.child( element, ACTIONS);
		if( acts != null)
		{
			Iterator iter =Util.children( acts, ACTION).iterator();
			while( iter.hasNext())
			{
				Element a =(Element)iter.next();
				createTool( a, activity.getTset_());
			}
		}

		loadExtendedAttributes( activity.getExtendedAttributes(), Util.child( element,
				EXTENDED_ATTRIBUTES));

		return activity;
	}
	
	protected void createTool( Element element, ToolSet toolSet) throws XPDLParserException
	{
		Tool tool = new Tool( element.attributeValue( ACTION_DONE));

		loadActionPM( tool.getParams(), Util.children( element, ACTIONPM));
		
		toolSet.addTools( tool);
	}

	public Condition createCondition( Element element)
	{
		Condition condition = new BasicCondition();

		Element e = Util.child( element, CONDITION);
		if( e != null && e.hasContent())
		{
			Object obj =e.content().iterator().next();
			if( obj instanceof DefaultCDATA)
			{
				DefaultCDATA cd =(DefaultCDATA)obj;
				condition.setText( cd.getText());
			}
		}
		
		if( condition.getText().length() == 0)
			condition.setIsdefault( true);

		return condition;
	}
	
	// Transitions

	/**
	 * Load transitions from the given Element. The element should represent a
	 * collection of transitions (which is identified by the "Transitions"
	 * tag).
	 * 
	 * @param transitions
	 *            The list of transitions
	 * @param element
	 *            The "Transitions" element
	 * @throws XPDLParserException
	 */

	protected void loadTransitions( List transitions, Element element) throws XPDLParserException
	{
		if( element == null){ return; }

		loadTransitions( transitions, Util.children( element, TRANSITION));
	}

	/**
	 * Load the transition objects from the given list of "Transition"
	 * elements.
	 * 
	 * @param transitions
	 *            The list of transitions
	 * @param elements
	 *            The list of "Transition" elements
	 * @throws XPDLParserException
	 */

	protected void loadTransitions( List transitions, List elements) throws XPDLParserException
	{
		transitions.clear();

		Iterator iter = elements.iterator();
		while( iter.hasNext())
		{
			Element transitionElement = (Element) iter.next();
			transitions.add( createTransition( transitionElement));
		}
	}

	protected Transition createTransition( Element element) throws XPDLParserException
	{
		Transition transition = new BasicTransition( element.attributeValue( ID), element
				.attributeValue( NAME), element.attributeValue( FROM), element.attributeValue( TO));

		transition.setCondition( createCondition( element));

		// load extended attributes
		loadExtendedAttributes( transition.getExtendedAttributes(), Util.child( element, EXTENDED_ATTRIBUTES));

		return transition;
	}

	// Actual Parameters
	/**
	 * Load the actual parameters objects from the given list of
	 * "ActualParameter" elements.
	 * 
	 * @param actualParameters
	 *            The list of actual parameters
	 * @param elements
	 *            The list of "ActualParameter" elements
	 * @throws XPDLParserException
	 */
	protected void loadActionPM( Map actualParameters, List elements)
			throws XPDLParserException
	{
		actualParameters.clear();

		Iterator iter = elements.iterator();
		while( iter.hasNext())
		{
			Element actualParameterElement = (Element) iter.next();
			
			actualParameters.put( actualParameterElement.attributeValue( ACTIONPM_N),
					actualParameterElement.attributeValue( ACTIONPM_V));
		}
	}

	// External Attributes

	/**
	 * Load extended attributes from the given Element. The element should
	 * represent a collection of extended attributes (which is identified by
	 * the "ExtendedAttributes" tag).
	 * 
	 * @param extendedAttributes
	 *            The map of extended attributes
	 * @param element
	 *            The "ExtendedAttributes" element
	 * @throws XPDLParserException
	 */

	protected void loadExtendedAttributes( Map extendedAttributes, Element element)
			throws XPDLParserException
	{
		if( element == null) return;
		
		loadExtendedAttributes( extendedAttributes, Util.children( element, EXTENDED_ATTRIBUTE));
	}

	/**
	 * Load the extended attribute objects from the given list of
	 * "ExtendedAttribute" elements.
	 * 
	 * @param extendedAttributes
	 *            The map of extended attribute
	 * @param elements
	 *            The list of "ExtendedAttribute" elements
	 * @throws XPDLParserException
	 */

	protected void loadExtendedAttributes( Map extendedAttributes, List elements)
			throws XPDLParserException
	{
		extendedAttributes.clear();

		Iterator iter = elements.iterator();
		while( iter.hasNext())
		{
			Element extendedAttributeElement = (Element) iter.next();
			String name = extendedAttributeElement.attributeValue( NAME);
			String value = extendedAttributeElement.attributeValue( VALUE);
			extendedAttributes.put( name, value);
			log.info( "Added extended attribute [" + name + "=" + value + "]");
		}
	}
}
