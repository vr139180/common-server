package com.cms.designer.swingui.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.IllegalComponentStateException;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ContainerEvent;
import java.awt.event.ContainerListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.HashMap;
import java.util.Hashtable;

import javax.swing.AbstractAction;
import javax.swing.AbstractButton;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JRootPane;
import javax.swing.JSplitPane;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.LookAndFeel;
import javax.swing.RootPaneContainer;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.UIDefaults;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.event.MouseInputListener;
import javax.swing.plaf.ActionMapUIResource;
import javax.swing.plaf.ToolBarUI;
import javax.swing.plaf.UIResource;
import javax.swing.plaf.basic.BasicBorders;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEFrameBar;

/**
 * the basicframeBarUI is inherited frome the toolbarUI.the toolbaruI is an abstract calss
 * 改变了原有的UI属性后,可以在上面添加其他的组件,来实现窗口拖动和改编大小的功能.
 */
public class BasicFrameBarUI extends ToolBarUI implements SwingConstants
{
	private static int direction;
	private static String FOCUSED_COMP_INDEX = "JToolBar.focusedCompIndex";
	public static String frameProperty = "frameBarProperty",
		toolProperty = "toolBarProperty";
	private static String IS_ROLLOVER = "JToolBar.isRollover";
	private static Border rolloverBorder;
	private static Border nonRolloverBorder;
	private static Border nonRolloverToggleBorder;

	protected OBEFrameBar toolBar =null;
	private OBEDesigner parent =null;
	
	private String frameBarName;
	private boolean floating;
	private int floatingX;
	private int floatingY;
	private JFrame floatingFrame;
	private RootPaneContainer floatingtoolBar;
	protected DragWindow dragWindow;
	
	protected Container dockingSource =null;
	
	private int dockingSensitivity = 0;
	protected int focusedCompIndex = -1;

	protected Color dockingColor = null;
	protected Color floatingColor = null;
	protected Color dockingBorderColor = null;
	protected Color floatingBorderColor = null;

	protected MouseInputListener dockingListener;
	protected PropertyChangeListener propertyListener;

	protected ContainerListener toolBarContListener;
	protected FocusListener toolBarFocusListener;
	public String property, barProperty;

	protected String constraintBeforeFloating = BorderLayout.NORTH;

	// Rollover button implementation.
	private boolean rolloverBorders = false;
	private HashMap borderTable = new HashMap();
	private Hashtable rolloverTable = new Hashtable();
	private JLabel dragLabel;

	public BasicFrameBarUI(OBEDesigner parent, String name)
	{
		this.parent = parent;
		frameBarName = name;
		property =frameProperty;
	}

	public void installUI( JComponent c)
	{
		toolBar = (OBEFrameBar)c;

		installDefaults();
		installComponents();
		installListeners();
//		installKeyboardActions();

		// Initialize instance vars
		dockingSensitivity = 0;
		floating = false;
		floatingX = floatingY = 0;
		floatingtoolBar = null;
		setOrientation( toolBar.getOrientation());

		c.setOpaque(true);

		if (c.getClientProperty(FOCUSED_COMP_INDEX) != null)
		{
			focusedCompIndex =
				((Integer) (c.getClientProperty(FOCUSED_COMP_INDEX)))
					.intValue();
		}
	}

	public void uninstallUI(JComponent c)
	{
		uninstallDefaults();
		uninstallComponents();
		uninstallListeners();
//		uninstallKeyboardActions();

		if (isFloating() == true)
			setFloating(false, null);

		floatingtoolBar = null;
		dragWindow = null;
		dockingSource = null;

		c.putClientProperty(FOCUSED_COMP_INDEX, new Integer(focusedCompIndex));
	}

	protected void installDefaults()
	{
		LookAndFeel.installBorder(toolBar, "toolBar.border");
		LookAndFeel.installColorsAndFont(
			toolBar,
			"toolBar.background",
			"toolBar.foreground",
			"toolBar.font");

		if (dockingColor == null || dockingColor instanceof UIResource)
			dockingColor = UIManager.getColor("toolBar.dockingBackground");
		if (floatingColor == null || floatingColor instanceof UIResource)
			floatingColor = UIManager.getColor("toolBar.floatingBackground");
		if (dockingBorderColor == null
			|| dockingBorderColor instanceof UIResource)
			dockingBorderColor =
				UIManager.getColor("toolBar.dockingForeground");
		if (floatingBorderColor == null
			|| floatingBorderColor instanceof UIResource)
			floatingBorderColor =
				UIManager.getColor("toolBar.floatingForeground");

		// ToolBar rollover button borders
		Object rolloverProp = toolBar.getClientProperty(IS_ROLLOVER);
		if (rolloverProp != null)
		{
			rolloverBorders = ((Boolean)rolloverProp).booleanValue();
		}
		if (rolloverBorder == null)
		{
			rolloverBorder = createRolloverBorder();
		}
		if (nonRolloverBorder == null)
		{
			nonRolloverBorder = createNonRolloverBorder();
		}
		if (nonRolloverToggleBorder == null)
		{
			nonRolloverToggleBorder = createNonRolloverBorder();
		}

		setRolloverBorders(isRolloverBorders());
	}

	protected void uninstallDefaults()
	{
		LookAndFeel.uninstallBorder(toolBar);
		dockingColor = null;
		floatingColor = null;
		dockingBorderColor = null;
		floatingBorderColor = null;

		installNormalBorders(toolBar);

		rolloverBorder = null;
		nonRolloverBorder = null;
		nonRolloverToggleBorder = null;
	}

	protected void installComponents()
	{}

	protected void uninstallComponents()
	{}

	protected void installListeners()
	{
		dockingListener = createDockingListener();

		propertyListener = createPropertyListener(); // added in setFloating
		if (propertyListener != null)
		{
			toolBar.addPropertyChangeListener(propertyListener);
		}

		toolBarContListener = createToolBarContListener();
		if (toolBarContListener != null)
		{
			toolBar.addContainerListener(toolBarContListener);
		}

		toolBarFocusListener = createToolBarFocusListener();
		if (toolBarFocusListener != null)
		{
			Component[] components = toolBar.getComponents();
			for (int i = 0; i < components.length; ++i)
				components[i].addFocusListener(toolBarFocusListener);
		}
	}

	protected void uninstallListeners()
	{
		if (dockingListener != null)
		{
			toolBar.removeMouseMotionListener(dockingListener);
			toolBar.removeMouseListener(dockingListener);

			dockingListener = null;
		}

		if (propertyListener != null)
		{
			toolBar.removePropertyChangeListener(propertyListener);
			propertyListener = null; // removed in setFloating
		}

		if (toolBarContListener != null)
		{
			toolBar.removeContainerListener(toolBarContListener);
			toolBarContListener = null;
		}

		if (toolBarFocusListener != null)
		{
			//Remove focus listener from all components in toolBar
			Component[] components = toolBar.getComponents();
			for (int i = 0; i < components.length; ++i)
				components[i].removeFocusListener(toolBarFocusListener);

			toolBarFocusListener = null;
		}
	}

	protected void installKeyboardActions()
	{
		InputMap km =
			getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);

		SwingUtilities.replaceUIInputMap(
			toolBar,
			JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT,
			km);
		ActionMap am = getActionMap();

		if (am != null)
		{
			SwingUtilities.replaceUIActionMap(toolBar, am);
		}
	}

	InputMap getInputMap(int condition)
	{
		if (condition == JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT)
		{
			return (InputMap)UIManager.get("toolBar.ancestorInputMap");
		}
		return null;
	}

	ActionMap getActionMap()
	{
		ActionMap map = (ActionMap)UIManager.get("toolBar.actionMap");

		if (map == null)
		{
			map = createActionMap();
			if (map != null)
			{
				UIManager.getLookAndFeelDefaults().put(
					"toolBar.actionMap",
					map);
			}
		}
		return map;
	}

	ActionMap createActionMap()
	{
		ActionMap map = new ActionMapUIResource();
		map.put("navigateRight", new RightAction());
		map.put("navigateLeft", new LeftAction());
		map.put("navigateUp", new UpAction());
		map.put("navigateDown", new DownAction());
		return map;
	}

	protected void uninstallKeyboardActions()
	{
		SwingUtilities.replaceUIActionMap(toolBar, null);
		SwingUtilities.replaceUIInputMap(
			toolBar,
			JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT,
			null);
	}
	/** the action followed is the interface of the key board reaction
	 *
	 * @param direction
	 */
	protected void navigateFocusedComp(int direction)
	{
		int nComp = toolBar.getComponentCount();
		int j;
		// the following code isn't used now
		/*	switch (direction) {
				case EAST :
				case SOUTH :
		
					if (focusedCompIndex < 0 || focusedCompIndex >= nComp)
						break;
		
					j = focusedCompIndex + 1;
		
					while (j != focusedCompIndex) {
						if (j >= nComp)
							j = 0;
						Component comp = toolBar.getComponentAtIndex(j++);
		
						if (comp != null && comp.isFocusTraversable()) {
							comp.requestFocus();
							break;
						}
					}
		
					break;
		
				case WEST :
				case NORTH :
		
					if (focusedCompIndex < 0 || focusedCompIndex >= nComp)
						break;
		
					j = focusedCompIndex - 1;
		
					while (j != focusedCompIndex) {
						if (j < 0)
							j = nComp - 1;
						Component comp = toolBar.getComponentAtIndex(j--);
		
						if (comp != null && comp.isFocusTraversable()) {
							comp.requestFocus();
							break;
						}
					}
		
					break;
		
				default :
					break;
			}
			*/
	}

	/**
	 * Creates a rollover border for toolBar components. The
	 * rollover border will be installed if rollover borders are
	 * enabled.
	 * <p>
	 * Override this method to provide an alternate rollover border.
	 *
	 * @since 1.4
	 */
	protected Border createRolloverBorder()
	{
		UIDefaults table = UIManager.getLookAndFeelDefaults();
		return new CompoundBorder(
			new BasicBorders.RolloverButtonBorder(
				table.getColor("controlShadow"),
				table.getColor("controlDkShadow"),
				table.getColor("controlHighlight"),
				table.getColor("controlLtHighlight")),
			new BasicBorders.MarginBorder());
	}

	/**
	 * Creates the non rollover border for toolBar components. This
	 * border will be installed as the border for components added
	 * to the toolBar if rollover borders are not enabled.
	 * <p>
	 * Override this method to provide an alternate rollover border.
	 *
	 * @since 1.4
	 */
	protected Border createNonRolloverBorder()
	{
		UIDefaults table = UIManager.getLookAndFeelDefaults();
		return new CompoundBorder(
			new BasicBorders.ButtonBorder(
				table.getColor("Button.shadow"),
				table.getColor("Button.darkShadow"),
				table.getColor("Button.light"),
				table.getColor("Button.highlight")),
			new BasicBorders.MarginBorder());
	}

	/**
	 * Creates a non rollover border for Toggle buttons in the toolBar.
	 */

	private Border createNonRolloverToggleBorder()
	{
		UIDefaults table = UIManager.getLookAndFeelDefaults();
		return new CompoundBorder(
			new BasicBorders.RadioButtonBorder(
				table.getColor("ToggleButton.shadow"),
				table.getColor("ToggleButton.darkShadow"),
				table.getColor("ToggleButton.light"),
				table.getColor("ToggleButton.highlight")),
			new BasicBorders.MarginBorder());
	}

	protected RootPaneContainer createFloatingWindow( OBEFrameBar toolbar)
	{
		class ToolBarDialog extends JDialog
		{
			public ToolBarDialog(Frame owner, String title, boolean modal)
			{
				super(owner, title, modal);
			}
			public ToolBarDialog(Dialog owner, String title, boolean modal)
			{
				super(owner, title, modal);
			}

			/**Override createRootPane() to automatically resize
			 * the frame when contents change
			 *
			 */
			protected JRootPane createRootPane()
			{
				JRootPane rootPane = new JRootPane(){
					private boolean packing = false;
					public void validate()
					{
						super.validate();
						if (!packing)
						{
							packing = true;
							pack();
							packing = false;
						}
					}
				};
				return rootPane;
			}
			protected void processMouseEvent(MouseEvent e)
			{
			}
		}

		JDialog dialog;
		Window window = SwingUtilities.getWindowAncestor(toolbar);
		if (window instanceof Frame)
		{
			dialog = new ToolBarDialog((Frame)window, toolbar.getName(), false);
		}
		else if (window instanceof Dialog)
		{
			dialog =
				new ToolBarDialog((Dialog)window, toolbar.getName(), false);
		}
		else
		{
			dialog = new ToolBarDialog((Frame) null, toolbar.getName(), false);
		}
		dialog.setTitle(toolbar.getName());
		dialog.setResizable(true);

		WindowListener wl = createFrameListener();
		dialog.addWindowListener(wl);
		return dialog;
	}

	protected DragWindow createDragWindow(OBEFrameBar toolBar)
	{
		Window frame = null;
		if (toolBar != null)
			frame = (Window)parent;

		if (floatingtoolBar == null)
			floatingtoolBar = createFloatingWindow(toolBar);

		if (floatingtoolBar instanceof Window)
			frame = (Window)floatingtoolBar;

		DragWindow dragWindow = new DragWindow(frame);

		return dragWindow;
	}

	/**
	 * Returns a flag to determine whether rollover button borders
	 * are enabled.
	 *
	 * @return true if rollover borders are enabled; false otherwise
	 * @see #setRolloverBorders
	 * @since 1.4
	 */
	public boolean isRolloverBorders()
	{
		return rolloverBorders;
	}

	/**
	 * Sets the flag for enabling rollover borders on the toolBar and it will
	 * also install the apropriate border depending on the state of the flag.
	 *
	 * @param rollover if true, rollover borders are installed.
	 *	      Otherwise non-rollover borders are installed
	 * @see #isRolloverBorders
	 * @since 1.4
	*/
	public void setRolloverBorders(boolean rollover)
	{
		rolloverBorders = rollover;

		if (rolloverBorders)
		{
			installRolloverBorders(toolBar);
		}
		else
		{
			installNonRolloverBorders(toolBar);
		}
	}

	/**
	 * Installs rollover borders on all the child components of the JComponent.
	 * <p>
	 * This is a convenience method to call <code>setBorderToRollover</code>
	 * for each child component.
	 *
	 * @param c container which holds the child components (usally a JtoolBar)
	 * @see #setBorderToRollover
	 * @since 1.4
	 */
	protected void installRolloverBorders(JComponent c)
	{
		Component[] components = c.getComponents();

		for (int i = 0; i < components.length; ++i)
		{
			if (components[i] instanceof JComponent)
			{
				((JComponent)components[i]).updateUI();
				setBorderToRollover(components[i]);
			}
		}
	}

	/**
	 * Installs non-rollover borders on all the child components of the JComponent.
	 * A non-rollover border is the border that is installed on the child component
	 * while it is in the toolBar.
	 * <p>
	 * This is a convenience method to call <code>setBorderToNonRollover</code>
	 * for each child component.
	 *
	 * @param c container which holds the child components (usally a JtoolBar)
	 * @see #setBorderToNonRollover
	 * @since 1.4
	 */
	protected void installNonRolloverBorders(JComponent c)
	{
		/**Put non-rollover borders on buttons. These borders reduce the margin.
		 *
		 */
		Component[] components = c.getComponents();

		for (int i = 0; i < components.length; ++i)
		{
			if (components[i] instanceof JComponent)
			{
				((JComponent)components[i]).updateUI();
				setBorderToNonRollover(components[i]);
			}
		}
	}

	/**
	 * Installs normal borders on all the child components of the JComponent.
	 * A normal border is the original border that was installed on the child
	 * component before it was added to the toolBar.
	 * <p>
	 * This is a convenience method to call <code>setBorderNormal</code>
	 * for each child component.
	 *
	 * @param c container which holds the child components (usally a JtoolBar)
	 * @see #setBorderToNonRollover
	 * @since 1.4
	 */
	protected void installNormalBorders(JComponent c)
	{
		/** Put back the normal borders on buttons
		 *
		 */
		Component[] components = c.getComponents();

		for (int i = 0; i < components.length; ++i)
		{
			setBorderToNormal(components[i]);
		}
	}

	/**
	 * Sets the border of the component to have a rollover border which
	 * was created by <code>createRolloverBorder</code>.
	 *
	 * @param c component which will have a rollover border installed
	 * @see #createRolloverBorder
	 * @since 1.4
	 */
	protected void setBorderToRollover(Component c)
	{
		if (c instanceof AbstractButton)
		{
			AbstractButton b = (AbstractButton)c;

			Border border = (Border)borderTable.get(b);
			if (border == null || border instanceof UIResource)
			{
				borderTable.put(b, b.getBorder());
			}
			/** Don't set a border if the existing border is null
			 *
			 */
			if (b.getBorder() != null)
			{
				b.setBorder(rolloverBorder);
			}

			rolloverTable.put(
				b,
				b.isRolloverEnabled() ? Boolean.TRUE : Boolean.FALSE);
			b.setRolloverEnabled(true);
		}
	}

	/**
	 * Sets the border of the component to have a non-rollover border which
	 * was created by <code>createNonRolloverBorder</code>.
	 *
	 * @param c component which will have a non-rollover border installed
	 * @see #createNonRolloverBorder
	 * @since 1.4
	 */
	protected void setBorderToNonRollover(Component c)
	{
		if (c instanceof AbstractButton)
		{
			AbstractButton b = (AbstractButton)c;

			Border border = (Border)borderTable.get(b);
			if (border == null || border instanceof UIResource)
			{
				borderTable.put(b, b.getBorder());
			}
			if (b.getBorder() != null)
			{
				if (b instanceof JToggleButton)
				{
					((JToggleButton)b).setBorder(nonRolloverToggleBorder);
				}
				else
				{
					//Don't set a border if the existing border is null

					b.setBorder(nonRolloverBorder);
				}
			}
			rolloverTable.put(
				b,
				b.isRolloverEnabled() ? Boolean.TRUE : Boolean.FALSE);
			b.setRolloverEnabled(false);
		}
	}

	/**
	 * Sets the border of the component to have a normal border.
	 * A normal border is the original border that was installed on the child
	 * component before it was added to the toolBar.
	 *
	 * @param c component which will have a normal border re-installed
	 * @see #createNonRolloverBorder
	 * @since 1.4
	 */
	protected void setBorderToNormal(Component c)
	{
		if (c instanceof AbstractButton)
		{
			AbstractButton b = (AbstractButton)c;

			Border border = (Border)borderTable.remove(b);
			b.setBorder(border);

			Boolean value = (Boolean)rolloverTable.remove(b);
			if (value != null)
			{
				b.setRolloverEnabled(value.booleanValue());
			}
		}
	}

	public Dimension getMinimumSize(JComponent c)
	{
		return getPreferredSize(c);
	}

	public Dimension getPreferredSize(JComponent c)
	{
		return null;
	}

	public Dimension getMaximumSize(JComponent c)
	{
		return getPreferredSize(c);
	}

	public void setFloatingLocation(int x, int y)
	{
		floatingX = x;
		floatingY = y;
	}

	public boolean isFloating()
	{
		return floating;
	}
	
	/**
	 *setFloating 中的dockingSource 是放toolBar的parent
	 */
	public void setFloating(boolean b, Point p)
	{
		//  toolBar.setFloatable(false);//--------
		if (toolBar.isFloatable() == true)
		{
			if (dragWindow != null)
				dragWindow.setVisible(false);
			this.floating = b;
			if (b == true)
			{
				if (dockingSource == null)
					dockingSource = parent.getTabSplit();

				dockingSource.remove( toolBar);
				
				((JSplitPane)dockingSource).setDividerSize(0);
				
				if (dockingSource instanceof JSplitPane)
					((JSplitPane)dockingSource).setDividerSize(0);
				
				dockingSource.validate();
				dockingSource.getParent().validate();
				dockingSource.repaint();
				
				Point l = new Point();
				toolBar.getLocation(l);
				constraintBeforeFloating =
					calculateConstraint(dockingSource, l);
				if (propertyListener != null)
					UIManager.addPropertyChangeListener(propertyListener);
				if (floatingtoolBar == null)
					floatingtoolBar = createFloatingWindow(toolBar);

				floatingtoolBar.getContentPane().add( toolBar,BorderLayout.CENTER);
				setOrientation(JToolBar.HORIZONTAL); //---------------
				if (floatingtoolBar instanceof Window)
					 ((Window)floatingtoolBar).pack();
				if (floatingtoolBar instanceof Window)
					((Window)floatingtoolBar).setLocation(floatingX, floatingY);
				if (floatingtoolBar instanceof Window)
					 ((Window)floatingtoolBar).show();
			}
			else
			{
				if (floatingtoolBar == null)
					floatingtoolBar = createFloatingWindow(toolBar);
				if (floatingtoolBar instanceof Window)
					 ((Window)floatingtoolBar).setVisible(false);
				floatingtoolBar.getContentPane().remove(toolBar);

				if (dockingSource instanceof JSplitPane)
				{
					((JSplitPane)dockingSource).remove( toolBar);
					((JSplitPane)dockingSource).setDividerSize(0);
				}
				
				if (dockingSource == null)
					dockingSource = parent.getTabSplit();
				if (propertyListener != null)
					UIManager.removePropertyChangeListener(propertyListener);

				if (p != null)
				{
					direction = calculateDirection(p);
					int orientation = mapConstraintToOrientation(direction);
					setOrientation(orientation);
					parent.createSplitPane(direction);
				}
			}
			
			dockingSource.invalidate();
			Container dockingSourceParent = dockingSource.getParent();
			if (dockingSourceParent != null)
				dockingSourceParent.validate();

			dockingSource.repaint();
		}
	}

	private int mapConstraintToOrientation(int direction)
	{
		int orientation = toolBar.getOrientation();

		if (direction == EAST || direction == WEST)
			orientation = JToolBar.VERTICAL;
		else if (direction == NORTH || direction == SOUTH)
			orientation = JToolBar.HORIZONTAL;

		return orientation;
	}

	public void setOrientation(int orientation)
	{
		toolBar.setOrientation(orientation);

		if (dragWindow != null)
			dragWindow.setOrientation(orientation);
	}

	/**
	 * Gets the color displayed when over a docking area
	 */
	public Color getDockingColor()
	{
		return dockingColor;
	}

	/**
	 * Sets the color displayed when over a docking area
	 */
	public void setDockingColor(Color c)
	{
		this.dockingColor = c;
	}

	/**
	 * Gets the color displayed when over a floating area
	 */
	public Color getFloatingColor()
	{
		return floatingColor;
	}

	/**
	 * Sets the color displayed when over a floating area
	 */
	public void setFloatingColor(Color c)
	{
		this.floatingColor = c;
	}

	public boolean canDock(Component c, Point p)
	{
		boolean b = false;
		dockingSource = parent.getTabSplit();
		
		dockingSensitivity =
			(toolBar.getOrientation() == JToolBar.HORIZONTAL)
				? toolBar.getSize().height
				: toolBar.getSize().width;
		if (p.y < dockingSensitivity)
		{
			// the dragwindow is  at upwards to the tabsplit
			b = true;
		}
		if (p.y >= c.getSize().height - dockingSensitivity)
		{
			b = true;
		}
		if (p.x < dockingSensitivity)
		{
			b = true;
		}
		if (p.x >= c.getSize().width - dockingSensitivity)
		{
			b = true;
		}
		return b;
	}

	private int calculateDirection(Point p)
	{
		dockingSource = parent.getTabSplit();

		dockingSensitivity =
			(toolBar.getOrientation() == JToolBar.HORIZONTAL)
				? toolBar.getSize().height
				: toolBar.getSize().width;

		if (p.x < dockingSensitivity)
		{
			direction = WEST; // west
		}
		else if (p.y < dockingSensitivity)
		{
			direction = NORTH; //north
		}
		else if (p.x >= dockingSource.getSize().width - dockingSensitivity)
		{
			direction = EAST; //east
		}
		else if (p.y >= dockingSource.getSize().height - dockingSensitivity)
		{
			direction = SOUTH; // south
		}
		return direction;
	}

	/**
	 * 这里来决定toolbar放的位子
	 */
	private String calculateConstraint(Component c, Point p)
	{
		if (p == null)
			return constraintBeforeFloating;
		String s = BorderLayout.NORTH;

		dockingSensitivity =
			(toolBar.getOrientation() == JToolBar.HORIZONTAL)
				? toolBar.getSize().height
				: toolBar.getSize().width;

		if (p.x < dockingSensitivity)
			s = BorderLayout.WEST;
		else if (p.y < dockingSensitivity)
			s = BorderLayout.NORTH;
		else if (p.x >= dockingSource.getSize().width - dockingSensitivity)
			s = BorderLayout.EAST;
		else if (p.y >= dockingSource.getSize().height - dockingSensitivity)
			s = BorderLayout.SOUTH;

		return s;
	}

	/**
	 * 来判断放的位子以及方向
	 */
	private String getDockingConstraint(Component c, Point p)
	{
		if (p == null)
			return constraintBeforeFloating;
		String s = BorderLayout.NORTH;

		dockingSensitivity =
			(toolBar.getOrientation() == JToolBar.HORIZONTAL)
				? toolBar.getSize().height
				: toolBar.getSize().width;
		if (p.x < dockingSensitivity)
			s = BorderLayout.WEST;
		else if (p.y < dockingSensitivity)
			s = BorderLayout.NORTH;
		else if (p.x >= dockingSource.getSize().width - dockingSensitivity)
			s = BorderLayout.EAST;
		else if (p.y >= dockingSource.getSize().height - dockingSensitivity)
			s = BorderLayout.SOUTH;

		return s;
	}

	/**
	 * dragto is draw the retangle
	 */
	protected void dragTo(Point position, Point origin)
	{
		if (toolBar.isFloatable() == true)
		{
			try
			{
				if (dragWindow == null)
					dragWindow = createDragWindow( toolBar);

				Point offset = dragWindow.getOffset();
				if (offset == null)
				{
					Dimension size = toolBar.getPreferredSize();
					offset = new Point(size.width / 2, size.height / 2);
					dragWindow.setOffset(offset);
				}
				Point global =
					new Point(origin.x + position.x, origin.y + position.y);
				Point dragPoint =
					new Point(global.x - offset.x, global.y - offset.y);
				Point mousePoint = new Point(position);

				if (dockingSource == null)
					dockingSource = parent.getTabSplit();

				Point p = new Point(origin);
				SwingUtilities.convertPointFromScreen( p, toolBar.getParent());

				constraintBeforeFloating =
					calculateConstraint(dockingSource, p);
				Point dockingPosition = dockingSource.getLocation();
				Point comparisonPoint =
					new Point(
						global.x - dockingPosition.x,
						global.y - dockingPosition.y);
				if (canDock(dockingSource, comparisonPoint))
				{
					dragWindow.setBackground(getDockingColor());
					direction = calculateDirection(position);
					int orientation = mapConstraintToOrientation(direction);
					dragWindow.setOrientation(orientation);
					dragWindow.setBorderColor(dockingBorderColor);
				}
				else
				{
					dragWindow.setBackground(getFloatingColor());
					dragWindow.setOrientation(JToolBar.HORIZONTAL);
					dragWindow.setBorderColor(floatingBorderColor);
				}
				dragWindow.setLocation(dragPoint.x, dragPoint.y);
				if (dragWindow.isVisible() == false)
				{
					Dimension size = toolBar.getPreferredSize();
					dragWindow.setSize(size.width, size.height);
					dragWindow.setVisible(true);
				}
			}
			catch (IllegalComponentStateException e)
			{
				e.printStackTrace();
			}
		}
	}
	
	/**
	 *  floateat is to locate the toolbar
	 */
	protected void floatAt(Point position, Point origin)
	{
		if (toolBar.isFloatable() == true)
		{
			try
			{
				Point offset = dragWindow.getOffset();
				if (offset == null)
				{
					offset = position;
					dragWindow.setOffset(offset);

				}
				Point global =
					new Point(origin.x + position.x, origin.y + position.y);
				setFloatingLocation(global.x - offset.x, global.y - offset.y);
				if (dockingSource != null)
				{
					Point dockingPosition = dockingSource.getLocation();
					Point comparisonPoint =
						new Point(
							global.x - dockingPosition.x,
							global.y - dockingPosition.y);
					if (canDock(dockingSource, comparisonPoint))
						setFloating(false, position);
					else
						setFloating(true, null);
				}
				else
					setFloating(true, null);

				dragWindow.setOffset(null);
			}
			catch (IllegalComponentStateException e)
			{
				e.printStackTrace();
			}
		}
	}

	protected ContainerListener createToolBarContListener()
	{
		return new toolBarContListener();
	}

	public FocusListener createToolBarFocusListener()
	{
		return new toolBarFocusListener();
	}

	public PropertyChangeListener createPropertyListener()
	{
		return new PropertyListener();
	}

	protected MouseInputListener createDockingListener()
	{
		return new DockingListener(toolBar);
	}

	public MouseInputListener createdockListener()
	{
		return new DockingListener(toolBar);
	}
	
	public ContainerListener createtoolBarContListener()
	{
		return new toolBarContListener();
	}
	
	public WindowListener createFrameListener()
	{
		return new FrameListener();
	}
	
	/**set the framebar invisialbe,remove the flowing bar or add the toolbar on the panel
	 */
	public void setUnFloating()
	{
		Point l = new Point();
		constraintBeforeFloating = calculateConstraint(dockingSource, l);
		String constraint = constraintBeforeFloating;
		parent.createSplitPane(3);
		parent.getTabSplit().repaint();
	}

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	private static abstract class KeyAction extends AbstractAction
	{
		public boolean isEnabled()
		{
			return true;
		}
	};

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	private static class RightAction extends KeyAction
	{
		public void actionPerformed(ActionEvent e)
		{
			JToolBar toolBar = (JToolBar)e.getSource();
			BasicFrameBarUI ui = (BasicFrameBarUI)toolBar.getUI();
			ui.navigateFocusedComp(EAST);
		}
	};

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	private static class LeftAction extends KeyAction
	{
		public void actionPerformed(ActionEvent e)
		{
			JToolBar toolBar = (JToolBar)e.getSource();
			BasicFrameBarUI ui = (BasicFrameBarUI)toolBar.getUI();
			ui.navigateFocusedComp(WEST);
		}
	};

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	private static class UpAction extends KeyAction
	{
		public void actionPerformed(ActionEvent e)
		{
			JToolBar toolBar = (JToolBar)e.getSource();
			BasicFrameBarUI ui = (BasicFrameBarUI)toolBar.getUI();
			ui.navigateFocusedComp(NORTH);
		}
	};

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	private static class DownAction extends KeyAction
	{
		public void actionPerformed(ActionEvent e)
		{
			JToolBar toolBar = (JToolBar)e.getSource();
			BasicFrameBarUI ui = (BasicFrameBarUI)toolBar.getUI();
			ui.navigateFocusedComp(SOUTH);
		}
	};

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	protected class FrameListener extends WindowAdapter
	{
		public void windowClosing(WindowEvent w)
		{
			if (toolBar.isFloatable() == true)
			{
				if (dragWindow != null)
					dragWindow.setVisible(false);
				floating = false;
				if (floatingtoolBar == null)
					floatingtoolBar = createFloatingWindow(toolBar);
				if (floatingtoolBar instanceof Window)
					 ((Window)floatingtoolBar).setVisible(false);
				floatingtoolBar.getContentPane().remove(toolBar);
				int orientation = mapConstraintToOrientation(direction);
				setOrientation(orientation);
				if (dockingSource == null)
					dockingSource = parent.getTabSplit();
				if (propertyListener != null)
					UIManager.removePropertyChangeListener(propertyListener);
				dockingSource.invalidate();
				Container dockingSourceParent = dockingSource.getParent();
				if (dockingSourceParent != null)
					dockingSourceParent.validate();
				dockingSource.repaint();
			}
		}

	}

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	protected class toolBarContListener implements ContainerListener
	{
		public void componentAdded(ContainerEvent e)
		{
			Component c = e.getChild();

			if (toolBarFocusListener != null)
			{
				c.addFocusListener(toolBarFocusListener);
			}

			if (isRolloverBorders())
			{
				setBorderToRollover(c);
			}
			else
			{
				setBorderToNonRollover(c);
			}
		}
		public void componentRemoved(ContainerEvent e)
		{
			Component c = e.getChild();
			if (toolBarFocusListener != null)
			{
				c.removeFocusListener(toolBarFocusListener);
			}
			setBorderToNormal(c);
		}
	}

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	protected class toolBarFocusListener implements FocusListener
	{
		public void focusGained(FocusEvent e)
		{
			Component c = e.getComponent();

			focusedCompIndex = toolBar.getComponentIndex(c);
		}

		public void focusLost(FocusEvent e)
		{}

	} // end class toolBarFocusListener

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	protected class PropertyListener implements PropertyChangeListener
	{
		public void propertyChange(PropertyChangeEvent e)
		{
			String propertyName = e.getPropertyName();
			if (propertyName.equals("lookAndFeel"))
			{
				toolBar.updateUI();
			}
			else if (propertyName.equals("orientation"))
			{
				// Search for JSeparator components and change it's orientation
				// to match the toolBar and flip it's orientation.
				Component[] components = toolBar.getComponents();
				int orientation = ((Integer)e.getNewValue()).intValue();
				JToolBar.Separator separator;

				for (int i = 0; i < components.length; ++i)
				{
					if (components[i] instanceof JToolBar.Separator)
					{
						separator = (JToolBar.Separator)components[i];
						separator.setOrientation(orientation);
						Dimension size = separator.getSize();
						if (size.width != size.height)
						{
							// Flip the orientation.
							Dimension newSize =
								new Dimension(size.height, size.width);
							separator.setSeparatorSize(newSize);
						}
					}
				}
			}
			else if (propertyName.equals(IS_ROLLOVER))
			{
				setRolloverBorders(((Boolean)e.getNewValue()).booleanValue());
			}
		}
	}

	/**
	 * This inner class is marked &quot;public&quot; due to a compiler bug.
	 * This class should be treated as a &quot;protected&quot; inner class.
	 * Instantiate it only within subclasses of BasictoolBarUI.
	 */
	public class DockingListener implements MouseInputListener
	{
		protected OBEFrameBar toolBar;

		protected boolean isDragging = false;
		protected Point origin = null;
		public Point mousePoint;

		public DockingListener(OBEFrameBar t)
		{
			this.toolBar = t;
		}
		public void mouseClicked(MouseEvent e)
		{}
		public void mousePressed(MouseEvent e)
		{
			if (!toolBar.isEnabled())
				return;
			isDragging = false;
		}
		public void mouseReleased(MouseEvent e)
		{
			if (!toolBar.isEnabled())
				return;
			if (isDragging == true)
			{
				Point position = e.getPoint();
				if (origin == null)
					origin = e.getComponent().getLocationOnScreen();
				floatAt(position, origin);
			}
			origin = null;
			isDragging = false;
		}
		public void mouseEntered(MouseEvent e)
		{}
		public void mouseExited(MouseEvent e)
		{}
		public void mouseDragged(MouseEvent e)
		{
			if (!toolBar.isEnabled())
				return;

			mousePoint = e.getPoint();
			isDragging = true;
			Point position = e.getPoint();
			if (origin == null)
				origin = e.getComponent().getLocationOnScreen();
			dragTo(position, origin);
		}
		public void mouseMoved(MouseEvent e)
		{}
	}
	
	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	protected class DragWindow extends Window
	{
		Color borderColor = Color.gray;
		int orientation = toolBar.getOrientation();
		Point offset;

		DragWindow(Window w)
		{
			super(w);
		}

		public void setOrientation(int o)
		{
			if (isShowing())
			{
				if (o == this.orientation)
					return;
				this.orientation = o;
				Dimension size = getSize();

				setSize(new Dimension(size.height, size.width));
				if (offset != null)
				{
					if (true)
					{
						setOffset(new Point(offset.y, offset.x));
					}
					else if (o == JToolBar.HORIZONTAL)
					{
						setOffset(new Point(size.height - offset.y, offset.x));
					}
					else
					{
						setOffset(new Point(offset.y, size.width - offset.x));
					}
				}
				repaint();
			}
		}
		public Point getOffset()
		{
			return offset;
		}
		public void setOffset(Point p)
		{
			this.offset = p;
		}
		public void setBorderColor(Color c)
		{
			if (this.borderColor == c)
				return;
			this.borderColor = c;
			repaint();
		}
		public Color getBorderColor()
		{
			return this.borderColor;
		}
		public void paint(Graphics g)
		{
			Color temp = g.getColor();
			g.setColor(getBackground());

			Dimension size = getSize();
			g.fillRect(0, 0, size.width, size.height);
			g.setColor(getBorderColor());
			g.drawRect(0, 0, size.width - 1, size.height - 1);

			super.paint(g);
		}
		public Insets getInsets()
		{
			return new Insets(1, 1, 1, 1);
		}
	}
}
