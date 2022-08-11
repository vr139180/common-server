package com.cms.core.workflow.serializer.dom4j;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.dom4j.Document;
import org.dom4j.DocumentFactory;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.cms.core.util.log.LogManager;
import com.cms.core.util.log.Logger;
import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.core.workflow.activity.Tool;
import com.cms.core.workflow.activity.ToolSet;

import com.cms.core.workflow.condition.Condition;
import com.cms.core.workflow.parser.ExtensionNames;
import com.cms.core.workflow.serializer.XPDLSerializer;
import com.cms.core.workflow.serializer.XPDLSerializerException;

/**
 * Implementation of the XPDLSerializer interface which uses the Dom4J library
 * to serialize a package to an XPDL document.
 */
public class Dom4JXPDLSerializer implements XPDLSerializer, ExtensionNames
{
	private static final Logger log = LogManager.getLogger( Dom4JXPDLSerializer.class.getName());

	/**
	 * Serialize the package to the given output stream.
	 * 
	 * @param pkg
	 *            The Package
	 * @param out
	 *            The OutputStream
	 * @throws IOException
	 *             Any I/O exception
	 * @throws XPDLSerializerException
	 *             Any serializer Exception
	 */
	public void serialize( WorkflowPackage pkg, OutputStream out) throws IOException,
			XPDLSerializerException
	{
		DocumentFactory df = new DocumentFactory();

		// serialize the Package
		Element packageElement = df.createElement( WORKFLOW_PROCESSES);

		writeWorkflowProcesses( pkg.getWorkflowProcesses(), packageElement, true);
		writeExtendedAttributes( pkg.getExtendedAttributes(), packageElement);

		// create the Document object
		Document document = df.createDocument( packageElement);

		// write the document to the output stream
		OutputFormat format = new OutputFormat( "  ", true);
		XMLWriter writer = new XMLWriter( out, format);
		writer.write( document);
		out.flush();
	}
	
	public void serialize2( WorkflowPackage pkg, OutputStream out) throws IOException, XPDLSerializerException
	{
		DocumentFactory df = new DocumentFactory();
		
		// serialize the Package
		Element packageElement = df.createElement( WORKFLOW_PROCESSES);
		
		writeWorkflowProcesses( pkg.getWorkflowProcesses(), packageElement, false);
		writeExtendedAttributes( pkg.getExtendedAttributes(), packageElement);
		
		Element e =com.cms.core.workflow.parser.dom4j.Util.child( packageElement, WORKFLOW_PROCESS);
		
		if( e != null)
		{
			// create the Document object
			Document document = df.createDocument( e);
			
			// write the document to the output stream
			OutputFormat format = new OutputFormat( "  ", true);
			format.setEncoding( "UTF-8");
			XMLWriter writer = new XMLWriter( out, format);
			writer.write( document);
			out.flush();			
		}
	}

	protected void writeWorkflowProcesses( List workflowProcesses, Element parent, boolean bext)
			throws XPDLSerializerException
	{
		if( workflowProcesses == null || workflowProcesses.size() == 0)
			return;
		
		Element bh =Util.addElement( parent, WORKFLOW_PROCESS);

		Iterator iter = workflowProcesses.iterator();
		if( iter.hasNext())
		{
			writeWorkflowProcess( (WorkflowProcess) iter.next(), bh, bext);
		}
	}

	protected void writeWorkflowProcess( WorkflowProcess wp, Element wpElement, boolean bext)
			throws XPDLSerializerException
	{
		wpElement.getParent().addAttribute( ID, wp.getId());
		wpElement.getParent().addAttribute( NAME, wp.getName());

		writeActivities( wp.getActivities(), wpElement, wp, bext);
		
		writeTransitions( wp.getTransitions(), wpElement.getParent());

		if( bext)
			writeWFExtendedAttributes( wp.getExtendedAttributes(), wpElement.getParent());
	}

	protected void writeWFExtendedAttributes( Map extendedAttributes, Element parent)
	{
		if( extendedAttributes == null || extendedAttributes.size() == 0)
			return;

		Element extendedAttributesElement = Util.addElement( parent, EXTENDED_ATTRIBUTES);

		Iterator keys = extendedAttributes.keySet().iterator();
		while( keys.hasNext())
		{
			Object key = keys.next();
			Object value = extendedAttributes.get( key);

			Element extendedAttributeElement = Util.addElement( extendedAttributesElement,
					EXTENDED_ATTRIBUTE);

			extendedAttributeElement.addAttribute( NAME, key.toString());
			extendedAttributeElement.addAttribute( VALUE, value.toString());
		}
	}

	protected void writeActivities( List activities, Element parent, WorkflowProcess wp, boolean bext) throws XPDLSerializerException
	{
		if( activities == null || activities.size() == 0)
			return;

		Iterator iter = activities.iterator();
		while( iter.hasNext())
		{
			Activity act =(Activity)iter.next();
			
			if( act.getActivityType() == Activity.START)
				parent.addAttribute( "s", act.getId());
			
			writeActivity( act, parent, wp, bext);
		}
	}

	protected void writeActivity( Activity activity, Element parent, WorkflowProcess wp, boolean bext) throws XPDLSerializerException
	{
		Element activityElement = Util.addElement( parent, ACTIVITY);

		activityElement.addAttribute( ID, activity.getId());
		activityElement.addAttribute( NAME, activity.getName());
		
		ToolSet ts =activity.getTset_();
		if( ts.getTools().size() > 0)
		{
			Element acts =Util.addElement( activityElement, ACTIONS);
			Map mm =ts.getTools();
			Iterator ff =mm.keySet().iterator();
			while( ff.hasNext())
			{
				String k =(String)ff.next();
				Tool vv =(Tool)mm.get( k);
				
				Element act =Util.addElement( acts, ACTION);
				act.addAttribute( ACTION_DONE, k);
				
				Map m2 =vv.getParams();
				Iterator ff2 =m2.keySet().iterator();
				while( ff2.hasNext())
				{
					String a =(String)ff2.next();
					String b =(String)m2.get( a);
					
					Element pm =Util.addElement( act, ACTIONPM);
					pm.addAttribute( ACTIONPM_N, a);
					pm.addAttribute( ACTIONPM_V, b);
				}
			}
		}
		
		Element fs =Util.addElement( activityElement, "fs");
		
		Iterator ff =wp.getTransitionsFrom( activity.getId()).iterator();
		while( ff.hasNext())
		{
			Transition t =(Transition)ff.next();
			
			if( t.isDefault())
			{
				fs.addAttribute( "def", t.getTo());
				continue;
			}
			
			Element f =Util.addElement( fs, "f");
			f.addAttribute( "to", t.getTo());
		
			Element c =Util.addElement( f, "c");
			if( t.getCondition().getText().length() > 0)
				c.addCDATA( t.getCondition().getText());
		}

		if( bext)
			writeExtendedAttributes( activity.getExtendedAttributes(), activityElement);
	}
	
	protected void writeTransitions( List transitions, Element parent)
			throws XPDLSerializerException
	{
		if( transitions == null || transitions.size() == 0)
			return;

		Element transitionsElement = Util.addElement( parent, TRANSITIONS);

		Iterator iter = transitions.iterator();
		while( iter.hasNext())
		{
			writeTransition( (Transition) iter.next(), transitionsElement);
		}
	}

	protected void writeTransition( Transition transition, Element parent)
			throws XPDLSerializerException
	{
		Element transitionElement = Util.addElement( parent, TRANSITION);

		transitionElement.addAttribute( ID, transition.getId());
		transitionElement.addAttribute( FROM, transition.getFrom());
		transitionElement.addAttribute( TO, transition.getTo());

		transitionElement.addAttribute( NAME, transition.getName());

		writeCondition( transition.getCondition(), transitionElement);

		writeExtendedAttributes( transition.getExtendedAttributes(), transitionElement);
	}

	protected void writeCondition( Condition condition, Element parent)
	{
		if( condition == null)
			return;

		Element conditionElement = Util.addElement( parent, CONDITION);
		if( condition.getText().length() > 0)
			conditionElement.addCDATA( condition.getText());
	}
	
	protected void writeExtendedAttributes( Map extendedAttributes, Element parent)
	{
		if( extendedAttributes == null || extendedAttributes.size() == 0)
			return;

		Element extendedAttributesElement = Util.addElement( parent, EXTENDED_ATTRIBUTES);

		Iterator keys = extendedAttributes.keySet().iterator();
		while( keys.hasNext())
		{
			String key = ( String)keys.next();
			Object value = extendedAttributes.get( key);

			Element extendedAttributeElement = Util.addElement( extendedAttributesElement,
					EXTENDED_ATTRIBUTE);

			extendedAttributeElement.addAttribute( NAME, key.toString());
			extendedAttributeElement.addAttribute( VALUE, value.toString());
		}
	}

}
