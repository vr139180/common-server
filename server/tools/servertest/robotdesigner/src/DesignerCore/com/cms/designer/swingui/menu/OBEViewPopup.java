package com.cms.designer.swingui.menu;

import java.awt.Dimension;

import javax.swing.JPopupMenu;
import javax.swing.JViewport;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEViewPanel;
import com.cms.designer.workflow.OBEGraphWorkflow;

/**
 * @author Administrator
 * view的比例缩放
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class OBEViewPopup extends JPopupMenu
{
	public static Dimension dim = new Dimension(100, 100);

	private OBEDesigner parent;
	private OBEGraphWorkflow graph;
	private OBEViewPanel viewPanel;
	private JViewport viewport;

	public OBEViewPopup(OBEDesigner parent, JViewport viewport, OBEGraphWorkflow graph)
	{
		super();
		
		this.parent = parent;
		this.graph = graph;
		this.viewport = viewport;
		this.setBorder(null);
		
		init();
	}

	/**
	 * 菜单界面初始化
	 */
	public void init()
	{
		viewPanel = new OBEViewPanel(parent, viewport, graph);
		this.add(viewPanel);
		this.setPreferredSize(dim);
		this.setPopupSize(dim);
	}
}
