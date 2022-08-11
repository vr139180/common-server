package com.cms.designer.swingui;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.awt.LayoutManager2;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.Serializable;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
import javax.accessibility.AccessibleRole;
import javax.accessibility.AccessibleStateSet;
import javax.swing.Action;
import javax.swing.BoxLayout;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JSeparator;
import javax.swing.JToolBar;
import javax.swing.SwingConstants;
import javax.swing.plaf.UIResource;

import com.cms.designer.swingui.ui.BasicFrameBarUI;

/**anthor steven
 * the framebar is a basic component of the designer 
 * it realize the function of floating window and frame.
 * it is inherited from Jcomponent ,and is resemble the toolBar
 * most of the function is referenced with JtoolBar
 */
public class OBEFrameBar
	extends JComponent
	implements SwingConstants, Accessible
{
	private static final String uiClassID = "ToolBarUI";
	private OBEDesigner parent;
	private boolean paintBorder = true;
	private Insets margin = null;
	private boolean floatable = true;
	private int orientation = HORIZONTAL;

	/**
	 * Creates a new tool bar; orientation defaults to <code>HORIZONTAL</code>.
	 */
	public OBEFrameBar(OBEDesigner parent)
	{
		this(parent, HORIZONTAL);
	}

	/**
	 * Creates a new tool bar with the specified <code>orientation</code>. 
	 * The <code>orientation</code> must be either <code>HORIZONTAL</code>
	 * or <code>VERTICAL</code>.
	 * 
	 * @param orientation  the orientation desired
	 */
	public OBEFrameBar(OBEDesigner parent, int orientation)
	{
		this(parent, null, orientation);
	}

	/**
	 * Creates a new tool bar with the specified <code>name</code>.  The 
	 * name is used as the title of the undocked tool bar.  The default
	 * orientation is <code>HORIZONTAL</code>.
	 * 
	 * @param name the name of the tool bar
	 * @since 1.3
	 */
	public OBEFrameBar(OBEDesigner parent, String name)
	{
		this(parent, name, HORIZONTAL);
	}

	/**
	 * Creates a new tool bar with a specified <code>name</code> and
	 * <code>orientation</code>.
	 * All other constructors call this constructor.
	 * If <code>orientation</code> is an invalid value, an exception will
	 * be thrown.
	 * 
	 * @param name  the name of the tool bar
	 * @param orientation  the initial orientation -- it must be
	 *		either <code>HORIZONTAL</code> or <code>VERTICAL</code>
	 * @exception IllegalArgumentException if orientation is neither
	 *		<code>HORIZONTAL</code> nor <code>VERTICAL</code>
	 * @since 1.3
	 */
	public OBEFrameBar(OBEDesigner parent, String name, int orientation)
	{
		this.parent = parent;
		setName(name);
		checkOrientation(orientation);
		this.orientation = orientation;
		DefaultToolBarLayout layout = new DefaultToolBarLayout(orientation);
		setLayout(layout);
		addPropertyChangeListener(layout);
		updateUI();
		addFrameItemListener itemListener = new addFrameItemListener();
	}

	/**
	 * Returns the tool bar's current UI.
	 * @see #setUI
	 */
	public BasicFrameBarUI getUI()
	{
		return (BasicFrameBarUI)ui;
	}

	/**
	 * Sets the L&F object that renders this component.
	 *
	 * @param ui  the <code>ToolBarUI</code> L&F object
	 * @see UIDefaults#getUI
	 * @beaninfo
	 *        bound: true
	 *       hidden: true
	 *    attribute: visualUpdate true
	 *  description: The UI object that implements the Component's LookAndFeel. 
	 */
	public void setUI(BasicFrameBarUI ui)
	{
		super.setUI(ui);
	}

	public OBEDesigner getDesignerParent()
	{
		return parent;
	}
	/**
	 * Returns the name of the L&F class that renders this component.
	 *
	 * @return the string "ToolBarUI"
	 * @see JComponent#getUIClassID
	 * @see UIDefaults#getUI
	 */
	public String getUIClassID()
	{
		return uiClassID;
	}

	/**
	 * Returns the index of the specified component.
	 * (Note: Separators occupy index positions.)
	 *
	 * @param c  the <code>Component</code> to find
	 * @return an integer indicating the component's position,
	 *		where 0 is first
	 */
	public int getComponentIndex(Component c)
	{
		int ncomponents = this.getComponentCount();
		Component[] component = this.getComponents();
		for (int i = 0; i < ncomponents; i++)
		{
			Component comp = component[i];
			if (comp == c)
				return i;
		}
		return -1;
	}

	/**
	 * Returns the component at the specified index.
	 *
	 * @param i  the component's position, where 0 is first
	 * @return   the <code>Component</code> at that position,
	 *		or <code>null</code> for an invalid index
	 *
	 */
	public Component getComponentAtIndex(int i)
	{
		int ncomponents = this.getComponentCount();
		if (i >= 0 && i < ncomponents)
		{
			Component[] component = this.getComponents();
			return component[i];
		}
		return null;
	}

	/**
	 * Sets the margin between the tool bar's border and
	 * its buttons. Setting to <code>null</code> causes the tool bar to
	 * use the default margins. The tool bar's default <code>Border</code>
	 * object uses this value to create the proper margin.
	 * However, if a non-default border is set on the tool bar, 
	 * it is that <code>Border</code> object's responsibility to create the
	 * appropriate margin space (otherwise this property will 
	 * effectively be ignored).
	 *
	 * @param m an <code>Insets</code> object that defines the space
	 * 	between the border and the buttons
	 * @see Insets
	 * @beaninfo
	 * description: The margin between the tool bar's border and contents
	 *       bound: true
	 *      expert: true
	 */
	public void setMargin(Insets m)
	{
		Insets old = margin;
		margin = m;
		firePropertyChange("margin", old, m);
		revalidate();
		repaint();
	}

	/**
	 * Returns the margin between the tool bar's border and
	 * its buttons.
	 *
	 * @return an <code>Insets</code> object containing the margin values
	 * @see Insets
	 */
	public Insets getMargin()
	{
		if (margin == null)
		{
			return new Insets(0, 0, 0, 0);
		}
		else
		{
			return margin;
		}
	}

	/**
	 * Gets the <code>borderPainted</code> property.
	 *
	 * @return the value of the <code>borderPainted</code> property
	 * @see #setBorderPainted
	 */
	public boolean isBorderPainted()
	{
		return paintBorder;
	}

	/**
	 * Sets the <code>borderPainted</code> property, which is
	 * <code>true</code> if the border should be painted.
	 * The default value for this property is <code>true</code>.
	 * Some look and feels might not implement painted borders;
	 * they will ignore this property.
	 *
	 * @param b if true, the border is painted
	 * @see #isBorderPainted
	 * @beaninfo
	 * description: Does the tool bar paint its borders?
	 *       bound: true
	 *      expert: true
	 */
	public void setBorderPainted(boolean b)
	{
		if (paintBorder != b)
		{
			boolean old = paintBorder;
			paintBorder = b;
			firePropertyChange("borderPainted", old, b);
			revalidate();
			repaint();
		}
	}

	/**
	 * Paints the tool bar's border if the <code>borderPainted</code> property
	 * is <code>true</code>.
	 * 
	 * @param g  the <code>Graphics</code> context in which the painting
	 * 	is done
	 * @see JComponent#paint
	 * @see JComponent#setBorder
	 */
	protected void paintBorder(Graphics g)
	{
		if (isBorderPainted())
		{
			super.paintBorder(g);
		}
	}

	/** 
	 * Gets the <code>floatable</code> property.
	 *
	 * @return the value of the <code>floatable</code> property
	 *
	 * @see #setFloatable
	 */
	public boolean isFloatable()
	{
		return floatable;
	}

	/**
	 * Sets the <code>floatable</code> property, 
	 * which must be <code>true</code> for the user to move the tool bar.
	 * Typically, a floatable tool bar can be 
	 * dragged into a different position within the same container
	 * or out into its own window.
	 * The default value of this property is <code>true</code>.
	 * Some look and feels might not implement floatable tool bars;
	 * they will ignore this property.
	 *
	 * @param b if <code>true</code>, the tool bar can be moved; 
	 *          <code>false</code> otherwise
	 * @see #isFloatable
	 * @beaninfo
	 * description: Can the tool bar be made to float by the user?
	 *       bound: true
	 *   preferred: true
	 */
	public void setFloatable(boolean b)
	{
		if (floatable != b)
		{
			boolean old = floatable;
			floatable = b;

			firePropertyChange("floatable", old, b);
			revalidate();
			repaint();
		}
	}

	/**
	 * Returns the current orientation of the tool bar.  The value is either
	 * <code>HORIZONTAL</code> or <code>VERTICAL</code>.
	 *
	 * @return an integer representing the current orientation -- either
	 *		<code>HORIZONTAL</code> or <code>VERTICAL</code>
	 * @see #setOrientation
	 */
	public int getOrientation()
	{
		return this.orientation;
	}

	/**
	 * Sets the orientation of the tool bar.  The orientation must have
	 * either the value <code>HORIZONTAL</code> or <code>VERTICAL</code>.
	 * If <code>orientation</code> is
	 * an invalid value, an exception will be thrown.
	 *
	 * @param o  the new orientation -- either <code>HORIZONTAL</code> or
	 *			</code>VERTICAL</code>
	 * @exception IllegalArgumentException if orientation is neither
	 *		<code>HORIZONTAL</code> nor <code>VERTICAL</code>
	 * @see #getOrientation
	 * @beaninfo
	 * description: The current orientation of the tool bar
	 *       bound: true
	 *   preferred: true
	 */
	public void setOrientation(int o)
	{
		checkOrientation(o);

		if (orientation != o)
		{
			int old = orientation;
			orientation = o;

			firePropertyChange("orientation", old, o);
			revalidate();
			repaint();
		}
	}

	/**
	 * Sets the rollover state of this toolbar. If the rollover state is true
	 * then the border of the toolbar buttons will be drawn only when the 
	 * mouse pointer hovers over them. The default value of this property
	 * is false.
	 * <p>
	 * The implementation of a look and feel may choose to ignore this 
	 * property.
	 * 
	 * @param rollover true for rollover toolbar buttons; otherwise false
	 * @since 1.4
	 * @beaninfo
	 *        bound: true
	 *    preferred: true
	 *    attribute: visualUpdate true
	 *  description: Will draw rollover button borders in the toolbar.
	 */
	public void setRollover(boolean rollover)
	{
		putClientProperty(
			"JToolBar.isRollover",
			rollover ? Boolean.TRUE : Boolean.FALSE);
	}

	/**
	 * Returns the rollover state.
	 * 
	 * @return true if rollover toolbar buttons are to be drawn; otherwise false
	 * @see #setRollover(boolean)
	 * @since 1.4
	 */
	public boolean isRollover()
	{
		Boolean rollover = (Boolean)getClientProperty("JToolBar.isRollover");
		if (rollover != null)
		{
			return rollover.booleanValue();
		}
		return false;
	}

	private void checkOrientation(int orientation)
	{
		switch (orientation)
		{
			case VERTICAL :
			case HORIZONTAL :
				break;
			default :
				throw new IllegalArgumentException("orientation must be one of: VERTICAL, HORIZONTAL");
		}
	}

	/**
	 * Appends a separator of default size to the end of the tool bar.
	 * The default size is determined by the current look and feel.
	 */
	public void addSeparator()
	{
		JToolBar.Separator s = new JToolBar.Separator();
		add(s);
	}

	/**
	 * Appends a separator of a specified size to the end
	 * of the tool bar.
	 *
	 * @param size the <code>Dimension</code> of the separator
	 */
	public void addSeparator(Dimension size)
	{
		JToolBar.Separator s = new JToolBar.Separator(size);
		add(s);
	}

	/**
	 * Adds a new <code>JButton</code> which dispatches the action.
	 *
	 * <p>
	 * As of 1.3, this is no longer the preferred method for adding
	 * <code>Action</code>s to a container.  Instead it is recommended
	 * to configure a control with an action using
	 * using <code>setAction</code>, and then add that control directly 
	 * to the <code>Container</code>.
	 *
	 * @param a the <code>Action</code> object to add as a new menu item
	 * @return the new button which dispatches the action
	 */
	public JButton add(Action a)
	{
		JButton b = createActionComponent(a);
		b.setAction(a);
		add(b);
		return b;
	}

	/**
	 * Factory method which creates the <code>JButton</code> for
	 * <code>Action</code>s added to the <code>JToolBar</code>.
	 * The default name is empty if a <code>null</code> action is passed.
	 *
	 * <p>
	 * As of 1.3, this is no longer the preferred method for adding
	 * <code>Action</code>s to a <code>Container</code>.
	 * Instead it is recommended to configure a control with an action
	 * using <code>setAction</code>, and then add that control directly 
	 * to the <code>Container</code>.
	 *
	 * @param a the <code>Action</code> for the button to be added
	 * @return the newly created button
	 * @see Action
	 */
	protected JButton createActionComponent(Action a)
	{
		String text = a != null ? (String)a.getValue(Action.NAME) : null;
		Icon icon = a != null ? (Icon)a.getValue(Action.SMALL_ICON) : null;
		boolean enabled = a != null ? a.isEnabled() : true;
		String tooltip =
			a != null ? (String)a.getValue(Action.SHORT_DESCRIPTION) : null;
		JButton b = new JButton(text, icon)
		{
			protected PropertyChangeListener createActionPropertyChangeListener(Action a)
			{
				PropertyChangeListener pcl = createActionChangeListener(this);
				if (pcl == null)
				{
					pcl = super.createActionPropertyChangeListener(a);
				}
				return pcl;
			}
		};
		if (icon != null)
		{
			b.putClientProperty("hideActionText", Boolean.TRUE);
		}
		b.setHorizontalTextPosition(JButton.CENTER);
		b.setVerticalTextPosition(JButton.BOTTOM);
		b.setEnabled(enabled);
		b.setToolTipText(tooltip);
		return b;
	}

	/**
	 * Returns a properly configured <code>PropertyChangeListener</code>
	 * which updates the control as changes to the <code>Action</code> occur,
	 * or <code>null</code> if the default
	 * property change listener for the control is desired.
	 *
	 * <p>
	 * As of 1.3, this is no longer the preferred method for adding
	 * <code>Action</code>s to a <code>Container</code>.
	 * Instead it is recommended to configure a control with an action
	 * using <code>setAction</code>, and then add that control directly 
	 * to the <code>Container</code>.
	 * @return <code>null</code>
	 */
	protected PropertyChangeListener createActionChangeListener(JButton b)
	{
		return null;
	}

	/**
	 * If a <code>JButton</code> is being added, it is initially
	 * set to be disabled.
	 *
	 * @param comp  the component to be enhanced
	 * @param constraints  the constraints to be enforced on the component
	 * @param index the index of the component
	 * 
	 */
	protected void addImpl(Component comp, Object constraints, int index)
	{
		super.addImpl(comp, constraints, index);
		if (comp instanceof JButton)
		{
			((JButton)comp).setDefaultCapable(false);
		}
	}

	/**
	 * A toolbar-specific separator. An object with dimension but
	 * no contents used to divide buttons on a tool bar into groups.
	 */
	static public class Separator extends JSeparator
	{
		private Dimension separatorSize;

		/** 
		 * Creates a new toolbar separator with the default size
		 * as defined by the current look and feel.
		 */
		public Separator()
		{
			this(null); // let the UI define the default size
		}

		/** 
		 * Creates a new toolbar separator with the specified size.
		*
		 * @param size the <code>Dimension</code> of the separator
		 */
		public Separator(Dimension size)
		{
			super(JSeparator.HORIZONTAL);
			setSeparatorSize(size);
		}

		/**
		* Returns the name of the L&F class that renders this component.
		*
		* @return the string "ToolBarSeparatorUI"
		* @see JComponent#getUIClassID
		* @see UIDefaults#getUI
		*/
		public String getUIClassID()
		{
			return "ToolBarSeparatorUI";
		}

		/** 
		 * Sets the size of the separator.
		 *
		 * @param size the new <code>Dimension</code> of the separator
		 */
		public void setSeparatorSize(Dimension size)
		{
			if (size != null)
			{
				separatorSize = size;
			}
			else
			{
				super.updateUI();
			}
			this.invalidate();
		}

		/** 
		 * Returns the size of the separator
		 *
		 * @return the <code>Dimension</code> object containing the separator's
		 *         size (This is a reference, NOT a copy!)
		 */
		public Dimension getSeparatorSize()
		{
			return separatorSize;
		}

		/** 
		 * Returns the minimum size for the separator.
		 *
		 * @return the <code>Dimension</code> object containing the separator's
		 *         minimum size
		 */
		public Dimension getMinimumSize()
		{
			return getPreferredSize();
		}

		/** 
		 * Returns the maximum size for the separator.
		 *
		 * @return the <code>Dimension</code> object containing the separator's
		 *         maximum size
		 */
		public Dimension getMaximumSize()
		{
			return getPreferredSize();
		}

		/** 
		 * Returns the preferred size for the separator.
		 *
		 * @return the <code>Dimension</code> object containing the separator's
		 *         preferred size
		 */
		public Dimension getPreferredSize()
		{
			return separatorSize.getSize();
		}
	}

	/** 
	 * See <code>readObject</code> and <code>writeObject</code> in
	 * <code>JComponent</code> for more 
	 * information about serialization in Swing.
	 */
	/*	private void writeObject(ObjectOutputStream s) throws IOException {
			s.defaultWriteObject();
			if (getUIClassID().equals(uiClassID)) {
				byte count = JComponent.getWriteObjCounter(this);
				JComponent.setWriteObjCounter(this, --count);
				if (count == 0 && ui != null) {
					ui.installUI(this);
				}
			}
		}
	*/

	/**
	 * Returns a string representation of this <code>JToolBar</code>.
	 * This method 
	 * is intended to be used only for debugging purposes, and the 
	 * content and format of the returned string may vary between      
	 * implementations. The returned string may be empty but may not 
	 * be <code>null</code>.
	 * 
	 * @return  a string representation of this <code>JToolBar</code>.
	 */
	protected String paramString()
	{
		String paintBorderString = (paintBorder ? "true" : "false");
		String marginString = (margin != null ? margin.toString() : "");
		String floatableString = (floatable ? "true" : "false");
		String orientationString =
			(orientation == HORIZONTAL ? "HORIZONTAL" : "VERTICAL");

		return super.paramString()
			+ ",floatable="
			+ floatableString
			+ ",margin="
			+ marginString
			+ ",orientation="
			+ orientationString
			+ ",paintBorder="
			+ paintBorderString;
	}

	private class DefaultToolBarLayout
		implements LayoutManager2, Serializable, PropertyChangeListener, UIResource
	{

		LayoutManager lm;

		DefaultToolBarLayout(int orientation)
		{
			if (orientation == OBEFrameBar.VERTICAL)
			{
				lm = new BoxLayout(OBEFrameBar.this, BoxLayout.PAGE_AXIS);
			}
			else
			{
				lm = new BoxLayout(OBEFrameBar.this, BoxLayout.LINE_AXIS);
			}
		}

		public void addLayoutComponent(String name, Component comp)
		{}

		public void addLayoutComponent(Component comp, Object constraints)
		{}

		public void removeLayoutComponent(Component comp)
		{}

		public Dimension preferredLayoutSize(Container target)
		{
			return lm.preferredLayoutSize(target);
		}

		public Dimension minimumLayoutSize(Container target)
		{
			return lm.minimumLayoutSize(target);
		}

		public Dimension maximumLayoutSize(Container target)
		{
			if (lm instanceof LayoutManager2)
			{
				return ((LayoutManager2)lm).maximumLayoutSize(target);
			}
			else
			{
				// Code copied from java.awt.Component.getMaximumSize()
				// to avoid infinite recursion.
				// See also java.awt.Container.getMaximumSize()
				return new Dimension(Short.MAX_VALUE, Short.MAX_VALUE);
			}
		}

		public void layoutContainer(Container target)
		{
			lm.layoutContainer(target);
		}

		public float getLayoutAlignmentX(Container target)
		{
			if (lm instanceof LayoutManager2)
			{
				return ((LayoutManager2)lm).getLayoutAlignmentX(target);
			}
			else
			{
				// Code copied from java.awt.Component.getAlignmentX()
				// to avoid infinite recursion.
				// See also java.awt.Container.getAlignmentX()
				return CENTER_ALIGNMENT;
			}
		}

		public float getLayoutAlignmentY(Container target)
		{
			if (lm instanceof LayoutManager2)
			{
				return ((LayoutManager2)lm).getLayoutAlignmentY(target);
			}
			else
			{
				// Code copied from java.awt.Component.getAlignmentY()
				// to avoid infinite recursion.
				// See also java.awt.Container.getAlignmentY()
				return CENTER_ALIGNMENT;
			}
		}

		public void invalidateLayout(Container target)
		{
			if (lm instanceof LayoutManager2)
			{
				((LayoutManager2)lm).invalidateLayout(target);
			}
		}

		public void propertyChange(PropertyChangeEvent e)
		{
			String name = e.getPropertyName();
			if (name.equals("orientation"))
			{
				int o = ((Integer)e.getNewValue()).intValue();

				if (o == JToolBar.VERTICAL)
					lm = new BoxLayout(OBEFrameBar.this, BoxLayout.PAGE_AXIS);
				else
				{
					lm = new BoxLayout(OBEFrameBar.this, BoxLayout.LINE_AXIS);
				}
			}
		}
	}

	public void setLayout(LayoutManager mgr)
	{
		LayoutManager oldMgr = getLayout();
		if (oldMgr instanceof PropertyChangeListener)
		{
			removePropertyChangeListener((PropertyChangeListener)oldMgr);
		}
		super.setLayout(mgr);
	}

	/**
	 *   Accessibility support
	 * 
	 */

	/**
	 * Gets the AccessibleContext associated with this JToolBar. 
	 * For tool bars, the AccessibleContext takes the form of an 
	 * AccessibleJToolBar. 
	 * A new AccessibleJToolBar instance is created if necessary.
	 *
	 * @return an AccessibleJToolBar that serves as the 
	 *         AccessibleContext of this JToolBar
	 */
	public AccessibleContext getAccessibleContext()
	{
		if (accessibleContext == null)
		{
			accessibleContext = new AccessibleJToolBar();
		}
		return accessibleContext;
	}

	/**
	 * This class implements accessibility support for the 
	 * <code>JToolBar</code> class.  It provides an implementation of the 
	 * Java Accessibility API appropriate to toolbar user-interface elements.
	 */
	protected class AccessibleJToolBar extends AccessibleJComponent
	{

		/**
		 * Get the state of this object.
		 *
		 * @return an instance of AccessibleStateSet containing the current 
		 * state set of the object
		 * @see AccessibleState
		 */
		public AccessibleStateSet getAccessibleStateSet()
		{
			AccessibleStateSet states = super.getAccessibleStateSet();
			// FIXME:  [[[WDW - need to add orientation from BoxLayout]]]
			// FIXME:  [[[WDW - need to do SELECTABLE if SelectionModel is added]]]
			return states;
		}

		/**
		 * Get the role of this object.
		 *
		 * @return an instance of AccessibleRole describing the role of the object
		 */
		public AccessibleRole getAccessibleRole()
		{
			return AccessibleRole.TOOL_BAR;
		}
	}
	/**inner class AccessibleJToolBar
	 * 
	 *
	 */
	public void addItemListener()
	{
		//public void windowClosed(WindowEvent w){
		//getDesignerParent().getOBEMenuBar().gettoolboxItem().setState(false);
		addFrameItemListener itemListener = new addFrameItemListener();

	}

	public class addFrameItemListener extends WindowAdapter
	{
		public void windowClosed(WindowEvent w)
		{
		}
	}
}
