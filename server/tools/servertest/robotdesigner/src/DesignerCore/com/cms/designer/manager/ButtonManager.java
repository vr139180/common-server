package com.cms.designer.manager;

import java.awt.Dimension;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Hashtable;
import java.util.Map;

import javax.swing.Action;
import javax.swing.JButton;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.ConstValue;

/**
 * @author zagio
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 * $Id: ButtonManager.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 * $Id: ButtonManager.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class ButtonManager implements PropertyNames
{
	//图标资源
	private IconManager iconManager = com.cms.designer.util.ResourceUtil.getIconManager();
	public static String IDE_TOOLBAR ="wf.ide.basic.toolbar";

	private OBEDesigner parent =null;
	
	private static Insets NO_MARGIN = new Insets(0, 0, 0, 0);
	private static Dimension size = new Dimension(30, 30);
	
	private Hashtable buttons =null;

	//文件菜单
	private JButton newButton;
	private JButton openButton;
	private JButton saveButton;
	private JButton saveAsButton;

	//编辑菜单
	private JButton cutButton;
	private JButton copyButton;
	private JButton pasteButton;
	private JButton redoButton;
	private JButton undoButton;
	
	//视图菜单
	private JButton zoomInButton;
	private JButton zoomOutButton;
	private JButton shapeAlignLeftButton;
	private JButton shapeAlignRightButton;
	private JButton shapeAlignCenterButton;
	private JButton shapeAlignTopButton;
	private JButton shapeAlignBottomButton;
	private JButton shapeAlignMiddleButton;

	//文本操作
	private JButton textPositionLeftButton;
	private JButton textPositionRightButton;
	private JButton textPositionCenterButton;

	public ButtonManager(OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}
	
	/**
	 * 初始化工具条
	 */
	private void init()
	{
		buttons = new Hashtable();
		Map actions = parent.getActions();

		//文件菜单
		newButton = createButton( ConstValue.CoreFileNew, actions.get( ConstValue.CoreFileNew));
		openButton = createButton( ConstValue.CoreFileOpen, actions.get( ConstValue.CoreFileOpen));
		saveButton = createButton( ConstValue.CoreFileSave, actions.get( ConstValue.CoreFileSave));
		saveAsButton = createButton( ConstValue.CoreFileSaveas, actions.get( ConstValue.CoreFileSaveas));

		//编辑菜单
		cutButton = createButton( ConstValue.CoreEditCut, actions.get( ConstValue.CoreEditCut));
		copyButton = createButton( ConstValue.CoreEditCopy, actions.get( ConstValue.CoreEditCopy));
		pasteButton = createButton( ConstValue.CoreEditPaste, actions.get( ConstValue.CoreEditPaste));
		redoButton = createButton( ConstValue.CoreEditRedo, actions.get( ConstValue.CoreEditRedo));
		undoButton = createButton( ConstValue.CoreEditUndo, actions.get( ConstValue.CoreEditUndo));

		saveButton.setEnabled(false);
		cutButton.setEnabled(false);
		copyButton.setEnabled(false);
		pasteButton.setEnabled(false);
		
		//视图菜单
		zoomInButton = createButton(ConstValue.CoreViewZoomin, actions.get(ConstValue.CoreViewZoomin));
		zoomOutButton = createButton(ConstValue.CoreViewZoomout, actions.get(ConstValue.CoreViewZoomout));
		shapeAlignLeftButton =
			createButton(SHAPEALIGNLEFT, actions.get("shape.alignLeft"));
		shapeAlignCenterButton =
			createButton(SHAPEALIGNCENTER, actions.get("shape.alignCenter"));
		shapeAlignRightButton =
			createButton(SHAPEALIGNRIGHT, actions.get("shape.alignRight"));
		shapeAlignTopButton =
			createButton(SHAPEALIGNTOP, actions.get("shape.alignTop"));
		shapeAlignMiddleButton =
			createButton(SHAPEALIGNMIDDLE, actions.get("shape.alignMiddle"));
		shapeAlignBottomButton =
			createButton(SHAPEALIGNBOTTOM, actions.get("shape.alignBottom"));

		zoomInButton.setEnabled(false);
		zoomOutButton.setEnabled(false);
		
		//文本操作
		textPositionLeftButton =
			createButton(TEXTPOSITIONLEFT, actions.get("textPosition.left"));
		textPositionCenterButton =
			createButton(TEXTPOSITIONCENTER,actions.get("textPosition.center"));
		textPositionRightButton =
			createButton(TEXTPOSITIONRIGHT, actions.get("textPosition.right"));
		
		loadResources();
	}
	
	/**
	 * 设置按钮属性
	 */
	public void loadResources()
	{
		load(newButton, "new.gif", ResourceUtil.getRS("window.new"));
		load(openButton, "folder.gif", ResourceUtil.getRS("window.open"));
		load(saveButton, "save.gif", ResourceUtil.getRS("window.save"));
		load(saveAsButton, "save_as.gif", ResourceUtil.getRS("window.saveAs"));

		load(cutButton, "cut.gif", ResourceUtil.getRS("window.cut"));
		load(copyButton, "copy.gif", ResourceUtil.getRS("window.copy"));
		load(pasteButton, "paste.gif", ResourceUtil.getRS("window.paste"));
		load(redoButton, "forward.gif", ResourceUtil.getRS("window.redo"));
		load(undoButton, "back.gif", ResourceUtil.getRS("window.undo"));

		load(zoomInButton, "zoomIn.gif", ResourceUtil.getRS("popup.zoomIn"));
		load(zoomOutButton, "zoomOut.gif", ResourceUtil.getRS("popup.zoomOut"));

		load(
			textPositionLeftButton,
			"textPositionLeft.gif",
			ResourceUtil.getRS("tagPopup.alignLeft"));

		load(
			textPositionRightButton,
			"textPositionRight.gif",
			ResourceUtil.getRS("tagPopup.alignRight"));

		load(
			textPositionCenterButton,
			"textPositionCenter.gif",
			ResourceUtil.getRS("tagPopup.alignCenter"));

		load(
			shapeAlignLeftButton,
			"alignLeft.gif",
			ResourceUtil.getRS("arrangment.left"));

		load(
			shapeAlignCenterButton,
			"alignCenter.gif",
			ResourceUtil.getRS("arrangment.center"));

		load(
			shapeAlignRightButton,
			"alignRight.gif",
			ResourceUtil.getRS("arrangment.right"));

		load(
			shapeAlignTopButton,
			"alignTop.gif",
			ResourceUtil.getRS("arrangment.top"));

		load(
			shapeAlignMiddleButton,
			"alignMiddle.gif",
			ResourceUtil.getRS("arrangment.middle"));

		load(
			shapeAlignBottomButton,
			"alignBottom.gif",
			ResourceUtil.getRS("arrangment.bottom"));
	}
	
	/**
	 * 为工具条按钮设置属性
	 * @param button
	 * @param iconName
	 * @param toolTipText
	 */
	private void load(JButton button, String iconName, String toolTipText)
	{
		button.setIcon(iconManager.getIcon(iconName));
		button.setToolTipText(toolTipText);
		button.setFocusPainted(false);
		button.setMargin(NO_MARGIN);
		button.setPreferredSize(size);
		button.setMinimumSize(size);
		button.setMaximumSize(size);
		button.setSize(size);
	}
	
	/**
	 * 创建菜单和事件的关联
	 * @param name
	 * @param obj
	 * @return
	 */
	private JButton createButton(String name, Object obj)
	{
		JButton button = new JButton((Action)obj);
		buttons.put(name, button);
		return button;
	}
	
	public Hashtable getButtons()
	{
		return buttons;
	}
	
	public JButton getByName(String name)
	{
		return (JButton)buttons.get(name);
	}
	
	public void setDefaultAction( String name)
	{
		JButton jb =( JButton)buttons.get( name);
		if( jb == null) return;
		Map actions = parent.getActions();
		if( !actions.containsKey( name))
			return;
		jb.setAction( (Action)actions.get( name));
	}

	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	class ComponentButtonAction extends MouseAdapter
	{
		public void mousePressed(MouseEvent e)
		{
			if (e.getButton() == MouseEvent.BUTTON1)
			{
				JButton button = (JButton)e.getSource();
				String key = button.getActionCommand();
				parent.getComponentbarManager().getToolBar().modifyBorder(key);
			}
		}
	}
}
