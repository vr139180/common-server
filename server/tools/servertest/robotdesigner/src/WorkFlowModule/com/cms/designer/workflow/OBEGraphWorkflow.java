package com.cms.designer.workflow;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import javax.swing.ImageIcon;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.undo.UndoableEdit;

import org.jgraph.JGraph;
import org.jgraph.graph.CellMapper;
import org.jgraph.graph.CellView;
import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.EdgeView;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphLayoutCache;
import org.jgraph.graph.Port;
import org.jgraph.graph.VertexView;

import com.anthonyeden.lib.gui.StandardAction;
import com.anthonyeden.lib.util.IconManager;
import com.cms.core.AbstractElement;
import com.cms.core.util.ResourceUtil;
import com.cms.core.util.log.LogManager;
import com.cms.core.util.log.Logger;
import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Activity;
import com.cms.core.workflow.activity.ToolSet;
import com.cms.core.workflow.parser.ExtensionNames;
import com.cms.core.workflow.transition.BasicTransition;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;
import com.cms.designer.swingui.menu.OBEGraphActivityModePopup;
import com.cms.designer.swingui.menu.OBEGraphEdgePopup;
import com.cms.designer.util.EditMode;
import com.cms.designer.util.ExtendedAttributeNames;
import com.cms.designer.util.ExtendedAttributeTools;
import com.cms.designer.util.Scale;
import com.cms.designer.view.ElementView;
import com.cms.designer.workflow.dialog.EditActivityDialog;
import com.cms.designer.workflow.dialog.EditTransitionDialog;
import com.cms.designer.workflow.dialog.EditWorkflowProcessDialog;
import com.cms.designer.workflow.dialog.NewActivityDialog;
import com.cms.designer.workflow.editor.ToolSetEditor;
import com.cms.designer.workflow.editor.WorkflowEditor;
import com.cms.designer.workflow.event.GraphKeyListener;
import com.cms.designer.workflow.graph.OBEEdge;
import com.cms.designer.workflow.graph.OBEGraphCell;
import com.cms.designer.workflow.graph.OBEGraphManager;
import com.cms.designer.workflow.graph.OBEGraphModel;
import com.cms.designer.workflow.graph.OBEMarqueeHandler;
import com.cms.designer.workflow.graph.OBEUndoManager;
import com.cms.designer.workflow.view.ActivityView;
import com.cms.designer.workflow.view.TransitionView;

/**
 * @author jeffery
 * 
 * OBEGraphWorkflow.java,v 1.11.2.4.2.3 2004/06/24 04:10:23 gaj Exp $
 */
public class OBEGraphWorkflow extends JGraph
		implements
			ExtendedAttributeNames,
			Printable,
			ExtensionNames
{
	private static final Logger log = LogManager.getLogger( OBEGraphWorkflow.class.getName());

	private static IconManager iconManager = com.cms.designer.util.ResourceUtil
			.getIconManager();

	private static final Point START_POINT = new Point( 100, 100);

	private static final Dimension DEFAULT_MINIMUM_SIZE = new Dimension( 600, 600);

	private static final int X_BASE = 50;

	private static final int Y_BASE = 16;

	public static final int X_WIDTH = 64;

	public static final int Y_WIDTH = 64;

	protected Point lastLocation = START_POINT;

	private GraphKeyListener keyListener = null;

	private OBEMarqueeHandler marqueeHandler;

	private OBEUndoManager undoManager;

	private OBEGraphManager graphManager;

	private boolean autoOPTEdge = false;

	private boolean pageVisible = false;

	protected Map elementViews;

	protected List transitionViews;

	protected Point currentPoint;

	protected OBEDesigner parent;

	protected OBEGraphModel graphModel;

	protected Map actions;

	protected Map editors;

	//引用 workflow process
	private WorkflowProcess workflowProcess;

	//引用 workflowPackage
	private WorkflowPackage workflowPackage;
	
	private OBEInternalFrame frame =null;

	private Object undoItem;
	private String key;

	public OBEGraphWorkflow( OBEDesigner parent, OBEGraphModel graphModel,String key)
	{
		super( graphModel);
		this.key=key;
		init( parent, graphModel);
	}

	public OBEGraphWorkflow( OBEDesigner parent, OBEGraphModel graphModel, GraphLayoutCache view)
	{
		super( graphModel, view);
		init( parent, graphModel);
	}

	private void init( OBEDesigner parent, OBEGraphModel graphModel)
	{
		setMinimumSize( DEFAULT_MINIMUM_SIZE);
		setCloneable( true);
		setDisconnectable( false);
		setBendable( false);
		getGraphLayoutCache().addObserver( new ViewObserver());
		graphManager = new OBEGraphManager( this);

		this.parent = parent;
		this.graphModel = graphModel;
		this.elementViews = new HashMap();
		this.transitionViews = new ArrayList();

		initEnv();
		installListeners();
		initActions();
		initEditors();
	}

	/**
	 * 各种监听器
	 */
	private void installListeners()
	{
		this.undoManager = graphManager.getUndoManager();

		this.marqueeHandler = graphManager.getMarqueeHandler();
		this.setMarqueeHandler( marqueeHandler);

		this.getModel().addUndoableEditListener( undoManager);

		keyListener = new GraphKeyListener( this);
		this.addKeyListener( keyListener);
	}

	private void initEnv()
	{
		this.gridVisible = true;
		this.gridSize = X_WIDTH / 4;
		this.gridEnabled = true;
		setPortsVisible( true);
		setTolerance( X_WIDTH / 4);
	}

	public void addTransition( Transition transition)
	{
		String fromId = transition.getFrom();
		String toId = transition.getTo();

		ElementView fromView = findElementViewById( fromId);
		ElementView toView = findElementViewById( toId);

		if( fromView == null || toView == null)
			return;
		connect( fromView, toView, transition);
	}

	public void addTransition( ElementView from, ElementView to, List points)
	{
		String fromId = from.getElement().getId();
		String toId = to.getElement().getId();
		if( fromId == null || toId == null)
			return;

		//首先判断是否已经存在链接form to 节点的transition
		List ff = workflowProcess.getTransitions();
		Transition tr = null;
		for( int i = 0; i < ff.size(); ++i)
		{
			tr = (Transition) ff.get( i);
			if( tr.getFrom().equals( fromId) && tr.getTo().equals( toId))
			{
					return;
			}
		}

		//获取transition id
		Transition transition = new BasicTransition( workflowProcess.getNextTransitionId(), "", fromId, toId);

		Map tmp = transition.getExtendedAttributes();

		writePoints( tmp, points);
		connect( from, to, transition);
		addToTransitions( transition);
		
		if( frame != null)
			frame.setFrameModified( true);
	}

	public void connect( ElementView from, ElementView to, Transition transition)
	{
		OBEGraphCell fromCell = (OBEGraphCell) from.getCell();
		OBEGraphCell toCell = (OBEGraphCell) to.getCell();
		connect( fromCell.getRightPort(), toCell.getLeftPort(), transition);
	}

	protected void connect( Port source, Port target, Transition transition)
	{
		if( isConnected( source, target))
			return;

		TransitionView view = new TransitionView( transition);
		transitionViews.add( view); /* @todo bad */
		ConnectionSet cs = new ConnectionSet();

		Map attributeMap = new HashMap();

		Map map = GraphConstants.createMap();
		GraphConstants.setLineStyle( map, GraphConstants.STYLE_ORTHOGONAL);
		GraphConstants.setLineEnd( map, GraphConstants.ARROW_SIMPLE);
		GraphConstants.setEndFill( map, true);
		GraphConstants.setBendable( map, true);
		GraphConstants.setEditable( map, false);
		Map extendedAttributes = transition.getExtendedAttributes();

		List points = new ArrayList();
		createPoints( extendedAttributes, points);

		GraphConstants.setPoints( map, points);

		OBEEdge edge = new OBEEdge( view);
		attributeMap.put( edge, map);
		cs.connect( edge, source, target);

		Object[] insert = {edge};
		view.setCell( edge);

		graphModel.insert( insert, attributeMap, cs, null, null);
	}

	private void writePoints( Map attributes, List points)
	{
		if( points.size() == 0)
			return;
		try
		{
			Point p_a = (Point) points.get( 0);
			if( p_a != null)
			{
				attributes.put( POINT_A_X, new Integer( p_a.x).toString());
				attributes.put( POINT_A_Y, new Integer( p_a.y).toString());
			}
			Point p_b = (Point) points.get( 1);
			if( p_b != null)
			{
				attributes.put( POINT_B_X, new Integer( p_b.x).toString());
				attributes.put( POINT_B_Y, new Integer( p_b.y).toString());
			}
		}
		catch( Exception e)
		{
			e.printStackTrace();
		}
	}

	protected ElementView findElementViewById( String id)
	{
		return (ElementView) elementViews.get( id);
	}

	/*
	 * public FontDescribe getTagFonts(Object cell) { TagGraphCell tagCell =
	 * (TagGraphCell)cell; String fontName =
	 * GraphConstants.getFont(tagCell.getAttributes()).getFamily(); String
	 * fontStyle =
	 * String.valueOf(GraphConstants.getFont(tagCell.getAttributes()).getStyle());
	 * String fontSize =
	 * String.valueOf(GraphConstants.getFont(tagCell.getAttributes()).getSize());
	 * Color color = GraphConstants.getForeground(tagCell.getAttributes());
	 * String foreColor = null; if (color == null) foreColor =
	 * ColorComboBox.getColorName(0); else foreColor =
	 * ColorComboBox.getName(GraphConstants.getForeground(tagCell.getAttributes()));
	 * FontDescribe fonts = new FontDescribe(fontName, fontStyle, fontSize,
	 * foreColor); return fonts; }
	 */

	public void redo()
	{
		undoManager.redo( getGraphLayoutCache());
	}

	public void undo()
	{
		undoManager.undo( getGraphLayoutCache());
	}

	public boolean canRedo()
	{
		return undoManager.canRedo( getGraphLayoutCache());
	}

	public boolean canUndo()
	{
		return undoManager.canUndo( getGraphLayoutCache());
	}

	public void updateHistoryButtons()
	{
	}

	public OBEDesigner getDesignerParent()
	{
		return parent;
	}

	public OBEGraphModel getOBEGraphModel()
	{
		return graphModel;
	}

	public EditMode getEditMode()
	{
		return parent.getEditMode();
	}

	public Map getActions()
	{
		return actions;
	}

	public void toLine()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRemoveAttributes( map, new Object[]{GraphConstants.ROUTING,
				GraphConstants.POINTS});
		setSelectionAttributes( map);
	}

	public void toBow()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRouting( map, GraphConstants.ROUTING_SIMPLE);
		GraphConstants.setRemoveAttributes( map, new Object[]{GraphConstants.POINTS});

		setSelectionAttributes( map);
	}

	public void edit()
	{
		try
		{
			Object cell = getSelectionCell();
			if( cell == null)
				editWorkflowProcess();
			if( cell instanceof OBEEdge)
				edit( (OBEEdge) cell);
			else if( cell instanceof DefaultGraphCell)
				edit( (DefaultGraphCell) cell);
			
			frame.setFrameModified( true);
		}
		catch( Throwable t)
		{
			t.printStackTrace();
		}
	}

	public void setSelectionAttributes( Map map)
	{
		Object selectionCells[] = DefaultGraphModel
				.getDescendants( getModel(), getSelectionCells()).toArray();

		UndoableEdit[] aundoableedit = null;
		ArrayList arraylist = new ArrayList();
		for( int i = 0; i < selectionCells.length; i++)
			if( !isPort( selectionCells[i]))
				arraylist.add( selectionCells[i]);

		selectionCells = arraylist.toArray();
		map = GraphConstants.cloneMap( map);

		map.remove( "bounds");
		map.remove( "points");
		Hashtable hashtable = new Hashtable();
		for( int j = 0; j < selectionCells.length; j++)
			hashtable.put( selectionCells[j], GraphConstants.cloneMap( map));

		getModel().edit( hashtable, null, null, null);
	}

	protected UndoableEdit[] augment( UndoableEdit aundoableedit[], UndoableEdit undoableedit)
	{
		if( undoableedit != null)
		{
			int i = aundoableedit == null ? 1 : aundoableedit.length + 1;
			UndoableEdit aundoableedit1[] = new UndoableEdit[i];
			if( aundoableedit != null)
				System.arraycopy( aundoableedit, 0, aundoableedit1, 0, i - 2);
			aundoableedit1[i - 1] = undoableedit;
			return aundoableedit1;
		}
		else
		{
			return aundoableedit;
		}
	}

	public void cleanUp()
	{
		/*
		 * if(route != null){ Iterator children =
		 * route.getChildren().iterator(); while(children.hasNext()){
		 * removeBounds((RouteNode)children.next()); }
		 * 
		 * setRoute(route); }
		 */
	}

	public void selectAll()
	{
		setSelectionCells( getRoots());
	}

	public void selectNone()
	{
		setSelectionCells( new Object[0]);
	}

	protected void createPoints( Map attributes, List points)
	{
		points.add( new Point( 10, 10)); //必须
		try
		{
			Integer p_a_x = ExtendedAttributeTools.getInteger( attributes, POINT_A_X);
			Integer p_a_y = ExtendedAttributeTools.getInteger( attributes, POINT_A_Y);

			Point p_a = new Point( p_a_x.intValue(), p_a_y.intValue());
			points.add( p_a);
		}
		catch( Exception e)
		{
		}
		try
		{
			Integer p_b_x = ExtendedAttributeTools.getInteger( attributes, POINT_B_X);
			Integer p_b_y = ExtendedAttributeTools.getInteger( attributes, POINT_B_Y);

			Point p_b = new Point( p_b_x.intValue(), p_b_y.intValue());
			points.add( p_b);
		}
		catch( Exception e)
		{
		}
		points.add( new Point( 10, 10));
	}

	protected void disconnectPort( Port port)
	{
		log.debug( "Disconnecting all edges on port " + port);
		if( port != null)
		{
			ArrayList edgesToRemove = new ArrayList();

			Iterator edges = graphModel.edges( port);
			while( edges.hasNext())
			{
				Object edge = edges.next();
				edgesToRemove.add( edge);
				log.debug( "Removing edge " + edge);
				port.removeEdge( edge);
			}

			log.debug( "Removing edges from model");
			Object[] remove = edgesToRemove.toArray( new Object[edgesToRemove.size()]);
			graphModel.remove( remove);
		}
	}

	protected void shiftOverlappingRight( Object cell)
	{
		Rectangle graphBounds = getBounds();
		Rectangle bounds = getCellBounds( cell);
		Object[] intersect = null;
		int x = bounds.x + bounds.width;
		int xShift = 0;

		ArrayList alreadyShifted = new ArrayList();

		// first shift all cells that intersect with the source cell
		intersect = getRoots( bounds);
		for( int i = 0; i < intersect.length; i++)
		{
			if( intersect[i] == cell)
				continue;

			if( intersect[i] instanceof DefaultGraphCell)
			{
				Rectangle intersectBounds = getCellBounds( intersect[i]);

				// the initial x
				int ox = intersectBounds.x;
				// update the x
				intersectBounds.x = x + X_BASE;
				// the diff betweeb the old and new x
				int dx = intersectBounds.x - ox;

				// the shift is the max difference
				if( dx > xShift)
					xShift = dx;

				alreadyShifted.add( intersect[i]);
			}
		}

		// now shift all cells which are to the right of the source cell
		Rectangle clip = new Rectangle( x, 0, graphBounds.width - x, graphBounds.height);

		intersect = getRoots( clip);
		for( int i = 0; i < intersect.length; i++)
		{
			if( alreadyShifted.contains( intersect[i]))
			{
				continue;
			}

			if( intersect[i] instanceof DefaultGraphCell)
			{
				Rectangle intersectBounds = getCellBounds( intersect[i]);
				intersectBounds.x += xShift;
			}
		}
	}

	protected void addCell( OBEGraphCell cell, Rectangle bounds, ImageIcon imageIcon)
	{
		Map attributeMap = new HashMap();

		Map map = GraphConstants.createMap();
		this.snap( bounds);
		checkScope( bounds);
		GraphConstants.setBounds( map, bounds);

		GraphConstants.setSizeable( map, false);
		GraphConstants.setBendable( map, false);

		GraphConstants.setIcon( map, imageIcon);
		GraphConstants.setOpaque( map, false);
		GraphConstants.setEditable( map, false);
		this.setLockedHandleColor( Color.green);

		attributeMap.put( cell, map);

		Object[] insert = {cell};
		graphModel.insert( insert, attributeMap, null, null, null);
		
		if( frame != null)
			frame.setFrameModified( true);
	}

	public int print( Graphics g, PageFormat pF, int page)
	{
		int pw = (int) pF.getImageableWidth();
		int ph = (int) pF.getImageableHeight();
		int cols = (int) (this.getWidth() / pw) + 1;
		int rows = (int) (this.getHeight() / ph) + 1;
		int pageCount = cols * rows;
		if( page >= pageCount)
			return NO_SUCH_PAGE;
		int col = page % cols;
		int row = page % rows;
		g.translate( -col * pw, -row * ph);
		g.setClip( col * pw, row * ph, pw, ph);

		paint( g);
		g.translate( col * pw, row * ph);
		return PAGE_EXISTS;
	}

	public void autoOPTEdge()
	{
		autoOPTEdge = (autoOPTEdge == true ? false : true);
	}

	public boolean isAutoOPTEdge()
	{
		return autoOPTEdge;
	}

	public boolean isPageVisible()
	{
		return pageVisible;
	}

	public void setPageVisible( boolean flag)
	{
		pageVisible = flag;
	}

	/**
	 * Returns the given Rectangle applied to the grid.
	 * 
	 * @param p
	 *            a Rectangle in screen coordinates.
	 * @return the same Rectangle applied to the grid.
	 */
	public Rectangle snap( Rectangle bounds)
	{
		Point p = bounds.getLocation();
		if( gridEnabled && p != null)
		{
			p.x += gridSize / 2;
			p.y += gridSize / 2;
			p.x = (int) Math.round( Math.round( p.x / gridSize) * gridSize);
			p.y = (int) Math.round( Math.round( p.y / gridSize) * gridSize);
		}
		bounds.setLocation( p);
		return bounds;
	}

	public Rectangle checkScope( Rectangle rc)
	{
		Point p = rc.getLocation();
		if( p.x < 0)
			p.x = gridSize / 2;
		if( p.y < 0)
			p.y = gridSize / 2;
		rc.setLocation( p);
		return rc;
	}

	public void addElement( DefaultGraphCell cell, Map extendedAttributes, Map attributeMap,
			Rectangle bounds, ImageIcon icon, boolean moveable, boolean sizeable)
	{
		Integer x = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_X);
		Integer y = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_Y);
		Integer width = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_WIDTH);
		Integer height = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_HEIGHT);

		// apply any bounds values in the extended attributes
		if( x != null)
			bounds.x = x.intValue();
		if( y != null)
			bounds.y = y.intValue();
		if( width != null)
			bounds.width = width.intValue();
		if( height != null)
			bounds.height = height.intValue();

		Map map = GraphConstants.createMap();
		snap( bounds);
		checkScope( bounds);
		GraphConstants.setBounds( map, bounds);
		GraphConstants.setOpaque( map, false);
		GraphConstants.setSizeable( map, sizeable);
		GraphConstants.setBendable( map, true);
		GraphConstants.setEditable( map, false);
		GraphConstants.setMoveable( map, moveable);

		if( icon != null)
			GraphConstants.setIcon( map, icon);

		GraphConstants.setBackground( map, Color.lightGray);
		attributeMap.put( cell, map);

		Object[] insert = {cell};
		graphModel.insert( insert, attributeMap, null, null, null);
	}

	public Map getElementViews()
	{
		return elementViews;
	}

	public Point getCurrentPoint()
	{
		return currentPoint;
	}

	public void setCurrentPoint( Point point)
	{
		currentPoint = point;
	}

	public boolean isGroup( Object cell)
	{
		// Map the Cell to its View
		CellView view = getGraphLayoutCache().getMapping( cell, false);
		if( view != null)
			return !view.isLeaf();
		return false;
	}

	//初始化 action map
	private void initActions()
	{
		actions = new HashMap();

		actions.put( "graph.autoOPTEdge", new StandardAction( this, "autoOPTEdge"));
		actions.put( "graph.toLine", new StandardAction( this, "toLine"));
		actions.put( "graph.toBow", new StandardAction( this, "toBow"));
		actions.put( "graph.edit", new StandardAction( this, "edit"));
		actions.put( "graph.delete", new StandardAction( this, "delete"));

		actions.put( "graph.snapHorizontal", new StandardAction( this, "snapHorizontal"));
		actions.put( "graph.snapVertical", new StandardAction( this, "snapVertical"));
		actions.put( "graph.snapLeft", new StandardAction( this, "snapLeft"));
		actions.put( "graph.snapRight", new StandardAction( this, "snapRight"));
		actions.put( "graph.snapSingle", new StandardAction( this, "snapSingle"));
		actions.put( "graph.snapFree", new StandardAction( this, "snapFree"));
		actions.put( "graph.reverse", new StandardAction( this, "reverse"));
		actions.put( "graph.showProperty", new StandardAction( this, "showProperty"));
		actions.put( "graph.actualSize", new StandardAction( this, "actualSize"));
		actions.put( "graph.zoomIn", new StandardAction( this, "zoomIn"));
		actions.put( "graph.zoomOut", new StandardAction( this, "zoomOut"));

		actions.put( "graph.addToolSet", new StandardAction( this, "addToolSet"));
		actions.put( "graph.addTransition", new StandardAction( this, "addTransition"));
	}

	//初始化 editor map
	private void initEditors()
	{
		editors = new HashMap();
		//editors.put( NoImplementation.class, new ManualActivityEditor());
		editors.put( ToolSet.class, new ToolSetEditor());
	}

	protected boolean isConnected( Port p1, Port p2)
	{
		Iterator edges = p1.edges();
		while( edges.hasNext())
		{
			Edge edge = (Edge) edges.next();
			if( edge.getTarget() == p2){ return true; }
		}
		return false;
	}

	public Point getLastLocation()
	{
		return lastLocation;
	}

	public void setLastLocation( Point point)
	{
		lastLocation = point;
	}

	public boolean isPort( Object obj)
	{
		return obj instanceof Port;
	}

	public void paintComponent( Graphics g)
	{
		super.paintComponent( g);
		graphManager.paint( g);
	}

	//水平折线
	public void snapHorizontal()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRouting( map, new OBEEdge.SnapHorizontalRouting());
		setSelectionAttributes( map);
		log.info( "snapHorizental");
	}

	//垂直折线
	public void snapVertical()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRouting( map, new OBEEdge.SnapVerticalRouting());
		setSelectionAttributes( map);
		log.info( "snapVertical");
	}

	//左折线
	public void snapLeft()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRouting( map, new OBEEdge.SnapLeftRouting());
		setSelectionAttributes( map);
		log.info( "snapLeft");
	}

	//右折线
	public void snapRight()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		GraphConstants.setRouting( map, new OBEEdge.SnapRightRouting());
		setSelectionAttributes( map);
		log.info( "snapRight");
	}

	//单折线
	public void snapSingle()
	{
		Object cell = getSelectionCell();
		OBEEdge edgeCell = (OBEEdge) cell;
		EdgeView edge = (EdgeView) getGraphLayoutCache().getMapping( edgeCell, false);
		Map map = GraphConstants.createMap();
		GraphConstants.setRemoveAttributes( map, new Object[]{GraphConstants.ROUTING});
		GraphConstants.setLineStyle( map, GraphConstants.STYLE_ORTHOGONAL);
		setSelectionAttributes( map);
		List points = edge.getPoints();
		Point p0 = edge.getPoint( 0);
		Point p1 = edge.getPoint( edge.getPointCount() - 1);
		while( points.size() > 2)
		{
			edge.removePoint( points.size() - 2);
		}
		Point centerPoint = fromScreen( snap( new Point( p0.x / 2 + p1.x / 2, p0.y / 2 + p1.y / 2)));
		edge.addPoint( 1, centerPoint);
		Object[] cells = graphLayoutCache.order( getSelectionCells());

		ConnectionSet cs = createConnectionSet( edge, edge.getCell(), false);
		Map nested = GraphConstants.createAttributes( new CellView[]{edge}, null);
		getGraphLayoutCache().edit( nested, cs, null, null);
		log.info( "snapSingle");
	}

	private ConnectionSet createConnectionSet( EdgeView view, Object edge, boolean verbose)
	{
		ConnectionSet cs = new ConnectionSet();
		Object sourcePort = null, targetPort = null;
		if( view.getSource() != null)
			sourcePort = view.getSource().getCell();
		if( view.getTarget() != null)
			targetPort = view.getTarget().getCell();
		if( verbose || sourcePort != getModel().getSource( edge))
			cs.connect( edge, sourcePort, true);
		if( verbose || targetPort != getModel().getTarget( edge))
			cs.connect( edge, targetPort, false);
		return cs;
	}

	private void setLineStyle()
	{
		Object cell = getSelectionCell();
		if( cell instanceof OBEEdge)
		{
			OBEEdge edgeCell = (OBEEdge) cell;
			GraphConstants.setLineStyle( edgeCell.getAttributes(), GraphConstants.STYLE_ORTHOGONAL);
		}
	}

	//自由线路
	public void snapFree()
	{
		Map map = GraphConstants.createMap();
		setLineStyle();
		Edge.Routing routing = (Edge.Routing) GraphConstants
				.getRouting( ((OBEEdge) getSelectionCell()).getAttributes());

		if( routing instanceof OBEEdge.SnapHorizontalRouting
				|| routing instanceof OBEEdge.SnapVerticalRouting)
		{
			GraphConstants.setRouting( map, new OBEEdge.SnapFreeRouting());
			setSelectionAttributes( map);
		}
		log.info( "snapFree");
	}

	//反向
	public void reverse()
	{
		if( getSelectionCount() == 1)
		{
			Object cell = getSelectionCell();
			if( cell instanceof OBEEdge)
			{
				OBEEdge edgeCell = (OBEEdge) cell;

				EdgeView view = (EdgeView) graphLayoutCache.getMapping( edgeCell, false);
				int count = view.getPointCount();
				List points = view.getPoints();
				Point[] ps = new Point[count];
				for( int i = 0; i < count; i++)
				{
					ps[i] = view.getPoint( count - 1 - i);
				}
				Map map = edgeCell.getAttributes();
				Object t = edgeCell.getTarget();
				Object s = edgeCell.getSource();
				edgeCell.setTarget( s);
				edgeCell.setSource( t);
				for( int i = 0; i < count; i++)
				{
					points.set( i, ps[i]);
				}
				GraphConstants.setPoints( map, points);
				setSelectionAttributes( map);

				DefaultPort fromPort = (DefaultPort) t;
				DefaultPort toPort = (DefaultPort) s;

				OBEGraphCell fromCell = (OBEGraphCell) fromPort.getParent();
				OBEGraphCell toCell = (OBEGraphCell) toPort.getParent();
				String from = null;
				String to = null;

				if( fromCell.getUserObject() instanceof ActivityView)
				{
					ActivityView fromView = (ActivityView) fromCell.getUserObject();
					ActivityView toView = (ActivityView) toCell.getUserObject();
					from = fromView.getActivity().getId();
					to = toView.getActivity().getId();
					if( toView.getActivity().getActivityType() == Activity.START)
					{
						JOptionPane.showMessageDialog( this, ResourceUtil
								.getRS( "message.message1"));
						return;
					}
					

				}

				Object userObject = edgeCell.getUserObject();
				log.debug( "User object type: " + userObject.getClass());
				if( userObject instanceof TransitionView)
				{
					TransitionView transitionView = (TransitionView) userObject;
					Transition transition = transitionView.getTransition();
					transition.setFrom( from);
					transition.setTo( to);
				}
			}
		}
		log.info( "reverse");
	}

	public void showProperty()
	{
		log.info( "showProperty");
	}

	public void zoomIn()
	{
		this.setScale( getScale() / Scale.RATE);
	}

	public void zoomOut()
	{
		this.setScale( getScale() * Scale.RATE);
	}

	public void actualSize()
	{
		this.setScale( 1.0);
	}

	/*
	 * (non-Javadoc) 向 graph 中添加元素
	 * 
	 * @see com.excenon.designer.workflow.graph.OBEGraphW#addComponent(java.lang.String)
	 */
	public void addComponent( String eventType)
	{
		Activity act = null;
		if( eventType.equals( OBEWorkflowModule.TOOLIMP))
		{
			//添加activity
			WorkflowEditor editor = new ToolSetEditor();
			act = addDefaultActivity( "活动节点", editor);
		}

		//去除needreplace的元素
		ActivityView av =( ActivityView)elementViews.get( "needreplace");
		//设置act的id
		if( act != null)
		{
			act.setId( workflowProcess.getNextActivityId());
			elementViews.put( act.getId(), av);
		}
	}

	/**
	 * Method addDefaultActivity.
	 * 
	 * @param editor
	 */
	public Activity addDefaultActivity( String name, WorkflowEditor editor)
	{
		//初始化值
		if( editor instanceof ToolSetEditor)
			editor.setWorkflowPackage( workflowPackage);

		Activity act = addActivity( editor, name);
		if( act == null)
			return act;

		//添加缺省设置
		if( editor instanceof ToolSetEditor)
		{
			act.initDefaultProperty();
		}

		return act;
	}

	//弹出菜单
	public void showPopup( Point p)
	{
		JPopupMenu popup = null;
		switch( getEditMode().getValue())
		{
		case EditMode.EDIT_TYPE:
			lastLocation = p;
			Object obj = getFirstCellForLocation( p.x, p.y);
			if( obj instanceof OBEEdge)
				popup = new OBEGraphEdgePopup( parent, this, obj, p);
			else if( obj instanceof OBEGraphCell)
				popup = new OBEGraphActivityModePopup( parent, (OBEGraphWorkflow) this, obj);
			else if( this instanceof OBEGraphWorkflow)
				popup = new OBEGraphActivityModePopup( parent, (OBEGraphWorkflow) this, obj);

			break;
		case EditMode.CONNECT_TYPE:
			break;
		case EditMode.VIEW_TYPE:
			break;
		}

		if( popup != null)
			popup.show( this, p.x, p.y);
	}

	/**
	 * 把activity添加到jgraph中去
	 * 
	 * @param activity
	 * @param editor
	 */
	public void addActivity( Activity activity, WorkflowEditor editor)
	{
		ActivityView activityView = new ActivityView( activity, editor);
		OBEGraphCell cell = new OBEGraphCell( activityView);

		//保存关联
		elementViews.put( activity.getId(), activityView);
		activityView.setCell( cell);

		ImageIcon imageIcon = null;

		if( editor instanceof ToolSetEditor)
		{
			if( activity.getActivityType() == Activity.START)
				imageIcon = (ImageIcon) iconManager.getIcon( "startone.gif");
			else
				imageIcon = (ImageIcon) iconManager.getIcon( "route" + getSubIconName( activity));
		}

		// set up a default bounds
		Rectangle bounds = new Rectangle( lastLocation.x, lastLocation.y, X_WIDTH, Y_WIDTH);

		// check for bounds values in the extended attributes
		Map extendedAttributes = activity.getExtendedAttributes();
		Integer x = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_X);
		if( x == null)
			extendedAttributes.put( BOUNDS_X, ""+bounds.x);
		Integer y = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_Y);
		if( y == null)
			extendedAttributes.put( BOUNDS_Y, ""+bounds.y);
		Integer width = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_WIDTH);
		if( width == null)
			extendedAttributes.put( BOUNDS_WIDTH, ""+bounds.width);
		Integer height = ExtendedAttributeTools.getInteger( extendedAttributes, BOUNDS_HEIGHT);
		if( height == null)
			extendedAttributes.put( BOUNDS_HEIGHT, ""+bounds.height);

		if( x != null)
			bounds.x = x.intValue();
		if( y != null)
			bounds.y = y.intValue();
		if( width != null)
			bounds.width = width.intValue();
		if( height != null)
			bounds.height = height.intValue();

		addCell( cell, bounds, imageIcon);
	}

	private String getSubIconName( Activity activity)
	{
		/*
		return "one2one.gif";
		return "one2multi.gif";
		return "multi2one.gif";
		*/
		return "multi2multi.gif";
	}

	//添加activity
	protected Activity addActivity( WorkflowEditor editor, String title)
	{
		Activity activity = null;
		NewActivityDialog d = new NewActivityDialog( this, editor, frame.getName());
		d.setTitle( title);
		if( d.showDialog() == NewActivityDialog.APPROVE_OPTION)
		{
			activity = d.getActivity();
			if( activity.getName() == null || activity.getName().equals( ""))
			{
				if( editor instanceof ToolSetEditor)
					activity.setName( ResourceUtil.getRS( "activity.activity"));
			}

			addActivity( activity, editor);
			workflowProcess.getActivities().add( activity);
		}

		return activity;
	}

	protected void addToTransitions( Transition transition)
	{
		workflowProcess.getTransitions().add( transition);
	}

	//编辑的方法
	public void edit( DefaultGraphCell graphCell)
	{
		Object userObject = graphCell.getUserObject();
		if( userObject == null)
			return;

		if( userObject instanceof ActivityView)
		{
			ActivityView activityView = (ActivityView) userObject;
			WorkflowEditor editor = activityView.getEditor();
			Activity activity = activityView.getActivity();

			if( editor != null)
				editor.setWorkflowPackage( workflowPackage);

			EditActivityDialog d = new EditActivityDialog( this, frame.getName(), activity, editor,
					this.workflowPackage, this.workflowProcess);
			if( d.showDialog() == EditActivityDialog.APPROVE_OPTION)
			{
			}
		}
	}

	//编辑流程
	public void editWorkflowProcess()
	{
		EditWorkflowProcessDialog d = new EditWorkflowProcessDialog( parent, workflowProcess,
				getName(), "编辑流程定义属性");
		d.showDialog();
	}

	public void edit( OBEEdge edge)
	{
		Object userObject = edge.getUserObject();
		if( userObject instanceof TransitionView)
		{
			TransitionView transitionView = (TransitionView) userObject;
			Transition transition = transitionView.getTransition();
			EditTransitionDialog d = new EditTransitionDialog( this, transition);
			d.showDialog();
		}
	}

	//删除操作
	public void delete()
	{
		Object cell = getSelectionCell();
		if( cell == null)
			return;

		if( cell instanceof OBEGraphCell)
			delete( (OBEGraphCell) cell);
		else if( cell instanceof OBEEdge)
			delete( (OBEEdge) cell);
	}

	public void delete( OBEGraphCell graphCell)
	{
		ArrayList cellsToRemove = new ArrayList();

		ActivityView activityView = (ActivityView) graphCell.getUserObject();
		Activity activity = activityView.getActivity();

		if( activity.getActivityType() == Activity.START)
			return;

		if (JOptionPane.showConfirmDialog(parent, "是否删除选择的活动", "警告", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE)
				== JOptionPane.NO_OPTION)
				return;

		deleteAllEdges( activity.getId());
		cellsToRemove.add( graphCell);

		Object[] remove = cellsToRemove.toArray( new Object[cellsToRemove.size()]);
		graphModel.remove( remove);

		workflowProcess.getActivities().remove( activity);
		
		frame.setFrameModified( true);
	}

	//删除连线
	private void deleteAllEdges( String activityID)
	{
		ArrayList edgesToRemove = new ArrayList();
		Iterator iter = transitionViews.iterator();

		while( iter.hasNext())
		{
			TransitionView transitionView = (TransitionView) iter.next();
			Transition transition = transitionView.getTransition();

			if( transition.getFrom().equals( activityID) || transition.getTo().equals( activityID))
			{

				Edge edge = (Edge) transitionView.getCell();
				edgesToRemove.add( edge);

				workflowProcess.getTransitions().remove( transition);
			}
		}

		Object[] remove = edgesToRemove.toArray( new Object[edgesToRemove.size()]);
		graphModel.remove( remove);
	}

	//删除连线
	public void delete( OBEEdge edge)
	{
		ArrayList cellsToRemove = new ArrayList();

		TransitionView transitionView = (TransitionView) edge.getUserObject();
		Transition transition = transitionView.getTransition();

		if (JOptionPane.showConfirmDialog(parent, "是否删除选择的条件", "警告", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE)
			== JOptionPane.NO_OPTION)
			return;

		cellsToRemove.add( edge);

		Object[] remove = cellsToRemove.toArray( new Object[cellsToRemove.size()]);
		graphModel.remove( remove);

		workflowProcess.getTransitions().remove( transition);
		
		frame.setFrameModified( true);
	}

	public WorkflowProcess getWorkflowProcess()
	{
		return workflowProcess;
	}

	public void setWorkflowProcess( WorkflowProcess workflowProcess)
	{
		this.workflowProcess = workflowProcess;
		graphModel.clear();
		

		Iterator activities = workflowProcess.getActivities().iterator();
		while( activities.hasNext())
		{
			Activity activity = (Activity) activities.next();
			Class implClass = activity.getTset_().getClass();

			addActivity( activity, (WorkflowEditor) editors.get( implClass));
		}

		Iterator transitions = workflowProcess.getTransitions().iterator();
		while( transitions.hasNext())
		{
			Transition transition = (Transition) transitions.next();
			log.debug( "Adding transition " + transition.getName());
			addTransition( transition);
		}

	}

	public void setWorkflowPackage( WorkflowPackage pkg)
	{
		this.workflowPackage = pkg;
	}

	/**
	 * 更新连接的id
	 * 
	 * @param oldId
	 * @param newId
	 */
	public void updateTransitions( String oldId, String newId)
	{
		Iterator iter = workflowProcess.getTransitions().iterator();
		while( iter.hasNext())
		{
			Transition t = (Transition) iter.next();
			if( t.getFrom().equals( oldId))
			{
				t.setFrom( newId);
			}
			if( t.getTo().equals( oldId))
			{
				t.setTo( newId);
			}
		}
	}

	private void updateActivityCellImage( Activity activity)
	{
		//修改图形
		ActivityView av = (ActivityView) elementViews.get( activity.getId());
		if( av == null)
			return;
		OBEGraphCell cell = (OBEGraphCell) av.getCell();
		//GraphConstants.get
		Map att = cell.getAttributes();

		ImageIcon imageIcon = null;
		if( activity.getActivityType() == Activity.START)
			imageIcon = (ImageIcon) iconManager.getIcon( "startone.gif");

		if( imageIcon != null)
			GraphConstants.setIcon( att, imageIcon);
		
		getGraphLayoutCache().reload();
		this.repaint();
	}

	private boolean canOperation( Activity activity)
	{
		String name = activity.getName();
		if( name.equals( ResourceUtil.getRS( "activity.start")))
			return false;
		else
			return true;
	}

	/**
	 * Method getTransitionViews.
	 * 
	 * @return Map
	 */
	public List getTransitionViews()
	{
		return transitionViews;
	}

	//覆盖jgraph 的方法 返回图形view的类型
	protected VertexView createVertexView( Object v, CellMapper cm)
	{
		return new VertexView( v, this, cm);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.excenon.designer.graph.OBEGraph#save(java.io.File)
	 */
	public void save( File f)
	{
		updateWorkflowElement();
	}

	/**
	 * 更新工作流单元的位置属性
	 */
	private void updateWorkflowElement()
	{
		Map m = this.getElementViews();
		if( m == null)
			return;
		Iterator ff = m.values().iterator();
		ElementView be = null;
		OBEGraphCell cel = null;
		AbstractElement bc = null;
		while( ff.hasNext())
		{
			be = (ElementView) ff.next();
			bc = (AbstractElement) be.getElement();
			if( !(bc instanceof Activity))
				continue;
			cel = (OBEGraphCell) be.getCell();
			Rectangle rc = this.getCellBounds( cel);
			if( rc == null)
				continue;
			int x = (int) rc.getX();
			int y = (int) rc.getY();
			//设置位置
			setActivityLocation( (Activity) bc, x, y);
		}
	}

	/**
	 * @param activity
	 * @param x
	 * @param y
	 */
	public void setActivityLocation( Activity activity, int x, int y)
	{
		activity.getExtendedAttributes().put( BOUNDS_X, "" + x);
		activity.getExtendedAttributes().put( BOUNDS_Y, "" + y);
		activity.getExtendedAttributes().put( BOUNDS_WIDTH, "" + X_WIDTH);
		activity.getExtendedAttributes().put( BOUNDS_HEIGHT, "" + Y_WIDTH);
	}

	/**
	 * @author Administrator
	 * 
	 * To change the template for this generated type comment go to Window -
	 * Preferences - Java - Code Generation - Code and Comments
	 */
	class ViewObserver implements Observer
	{
		public void update( Observable o, Object arg)
		{
			log.debug( "View changed: " + o);
			graphModel.setModified( true);
		}
	}

	public void cut()
	{
		Object cell = getSelectionCell();
		if( cell == null)
			return;
		undoItem = cell;

		if( cell instanceof OBEGraphCell)
			delete( (OBEGraphCell) cell);
	}

	public void copy()
	{
		Object cell = getSelectionCell();
		if( cell == null)
			return;
		undoItem = cell;
	}

	public void paste()
	{
		if( undoItem instanceof OBEGraphCell)
			pasteOBEGraphCell( (OBEGraphCell) undoItem);
		else if( undoItem instanceof OBEEdge)
			pasteOBEEdge( (OBEEdge) undoItem);
		
		frame.setFrameModified( true);
	}

	private void pasteOBEGraphCell( OBEGraphCell itemCell)
	{
		ActivityView activityView = (ActivityView) itemCell.getUserObject();
		Activity activity = activityView.getActivity();

		Class implClass = activity.getTset_().getClass();

		addActivity( activity, (WorkflowEditor) editors.get( implClass));
	}

	private void pasteOBEEdge( OBEEdge itemEdge)
	{
		//undo();
	}
	
	/**
	 * @param frame The frame to set.
	 */
	public void setFrame( OBEInternalFrame frame)
	{
		this.frame = frame;
	}
	/**
	 * @return Returns the frame.
	 */
	public OBEInternalFrame getFrame()
	{
		return frame;
	}
	/**
	 * @return Returns the key.
	 */
	public String getKey()
	{
		return key;
	}
}
