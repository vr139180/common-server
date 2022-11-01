package com.cms.designer.swingui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JTree;
import javax.swing.ToolTipManager;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.swingui.menu.*;
import com.cms.designer.util.OBETreeData;

/**
 * @author jeffery
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 * $Id: OBETree.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class OBETree extends JTree
{
	private static final Log log = LogFactory.getLog(OBETree.class);

	private OBEDesigner parent = null;

	//根节点
	private OBETreeNode rootNode =
		new OBETreeNode(new OBETreeData(ResourceUtil.getRS("tree.root"), OBETreeNode.ROOT, "frameIcon.gif"));

	//tree的数据模式
	private DefaultTreeModel treeModel;
	private TreePath rootPath;

	//当前选中的节点
	private OBETreeNode curSelectedNode = null;

	public OBETree(OBEDesigner parent)
	{
		super();

		this.parent = parent;
		treeModel = new DefaultTreeModel(rootNode);
		setShowsRootHandles(true);

		ToolTipManager.sharedInstance().registerComponent(this);

		this.setModel(treeModel);
		this.setCellRenderer(new OBETreeCellRenderer());

		this.addTreeSelectionListener(new TreeSelectionListener()
		{
			public void valueChanged(TreeSelectionEvent e)
			{
				//选择发生了变化
				TreePath oldT = e.getOldLeadSelectionPath();
				TreePath newT = e.getNewLeadSelectionPath();
				treeItemChange(oldT, newT);
			}
		});

		addMouseListener(new MouseAdapter()
		{
			public void mousePressed(MouseEvent e)
			{
				dealMousePressed( e);
			}
			public void mouseReleased(MouseEvent evt)
			{
				dealMouseReleased(evt);
			}
		});

		//保存根path
		rootPath = new TreePath(getModel().getRoot());
	}
	
	private void dealMousePressed( MouseEvent e)
	{
		if (e.getButton() == MouseEvent.BUTTON3)
		{
			Point pt =e.getPoint();
			TreePath tp =this.getPathForLocation( pt.x, pt.y);
			if( tp == null)
				return;
			this.setSelectionPath( tp);
		}
	}

	/**
	 * 在
	 * @param oldT
	 * @param newT
	 */
	private void treeItemChange(TreePath oldT, TreePath newT)
	{
		if( newT == null)	
			return;
		OBETreeNode tn = (OBETreeNode)newT.getLastPathComponent();
		if (tn == null)
			return;
		OBETreeData td = (OBETreeData)tn.getUserObject();
	}

	/**
	 * 处理鼠标释放事件
	 * @param evt
	 */
	private void dealMouseReleased(MouseEvent evt)
	{
		OBETreeNode node = (OBETreeNode)getLastSelectedPathComponent();
		if (node == null)
			return;

		if (evt.getClickCount() == 2)
		{
			//双击打开
			openModuleWindow(node);
		}
		else if (evt.isPopupTrigger())
		{
			//显示弹出式菜单--处理右键鼠标动作
			setCurSelectedNode(node);
			//处理弹出式菜单
			showPopup(node, evt.getPoint());
		}
	}

	/**
	 * 判断是否能够打开view
	 * @param key
	 * @return
	 */
	private boolean canOpenView(String key)
	{
		boolean ret = false;
		ElementKey ek = new ElementKey();
		ek.analyze(key);
		ModuleManager mm = OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms = mm.getModule(ek.getModuleID());
		if (ms == null)
			return ret;
		ret = ms.canOpenView(ek.getModuleExtAtt());

		return ret;
	}

	/**
	 * 处理双击tree节点打开窗口
	 * @param node
	 */
	public void openModuleWindow(OBETreeNode node)
	{
		//获取附加数据
		OBETreeData treeData = (OBETreeData)node.getUserObject();
		String type = treeData.getType();
		if (!type.equals(OBETreeNode.MODULE))
			return;
		//是否能够打开一个view窗口
		if (!canOpenView(treeData.getKey()))
			return;
		if (parent.getDesktopPanel().isOpen(treeData.getKey()))
		{
			//是否已经打开了一个窗口
			parent.getDesktopPanel().showFrame(treeData.getKey());
		}
		else
		{
			//创建一个新的
			//获取包信息
			ElementKey ek = new ElementKey();
			ek.analyze(treeData.getKey());
			ModuleManager manager = OBEModuleManager.getInstance().getModuleManager();
			OBEModuleStandard ms = manager.getModule(ek.getModuleID());
			if (ms == null)
				return;
			OBEInternalFrame frame = ms.newModuleFrame(treeData.getKey());
			if (frame != null)
				parent.getDesktopPanel().addInternalFrame(frame, true);
		}
	}

	public TreePath getRootPath()
	{
		return rootPath;
	}

	public void expandRoot()
	{
		TreeModel model = getModel();
		if (model != null && model.getRoot() != null)
		{
			expandPath(rootPath);
		}
	}

	public void expandNode(OBETreeNode node)
	{
		makeVisible(new TreePath(node.getPath()));
	}

	/**
	 * 测试是否可以改变视图
	 * @param type
	 * @return
	 */
	public boolean canChangeView(String type)
	{
		if (type.equals(OBETreeNode.WORKFLOW)
			|| type.equals(OBETreeNode.PACKAGE)
			|| type.equals(OBETreeNode.PROCESS))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Method showPopup.
	 * @param string
	 * @param point
	 */
	private void showPopup(OBETreeNode node, Point p)
	{
		OBETreePopupMenu menu = new OBETreePopupMenu(parent, node);
		if (menu.isShowit())
			menu.show(this, p.x, p.y);
	}

	/**
	 * Method modify.
	 */
	public void modify(OBETreeNode node)
	{
		((DefaultTreeModel)this.getModel()).nodeStructureChanged((TreeNode)node);
	}

	/**
	 * @return
	 */
	public OBETreeNode getRootNode()
	{
		return rootNode;
	}

	public void removeAllFromRoot()
	{
		rootNode.removeAllChildren();
	}

	/**
	 * 创建一个节点
	 * @param parentNode
	 * @param name
	 * @param type
	 * @param iconName
	 * @return
	 */
	public OBETreeNode createNode(OBETreeNode parentNode, String name, String type, String iconName)
	{
		OBETreeNode node = new OBETreeNode(new OBETreeData(name, type, iconName));
		parentNode.add(node);
		return node;
	}

	/**
	 * 增加一个节点
	 * @param td
	 */
	public OBETreeNode addProjectNode(OBETreeData td)
	{
		OBETreeNode node = new OBETreeNode(td);
		rootNode.add(node);
		this.updateUI();

		return node;
	}

	/**
	 * @param td
	 */
	public OBETreeNode addProjectModuleNode(OBETreeNode p, OBETreeData td)
	{
		OBETreeNode node = new OBETreeNode(td);
		p.add(node);
		TreePath tp =new TreePath( node.getPath());
		this.setSelectionPath( tp);
		this.updateUI();
		this.expandNode( node);

		return node;
	}

	public OBETreeNode getCurSelectedNode()
	{
		return curSelectedNode;
	}

	public void setCurSelectedNode(OBETreeNode node)
	{
		curSelectedNode = node;
	}
	
	/**
	 * @author Administrator
	 *
	 * To change the template for this generated type comment go to
	 * Window - Preferences - Java - Code Generation - Code and Comments
	 */
	static class OBETreeCellRenderer extends JLabel implements TreeCellRenderer
	{
		private static IconManager iconManager = com.cms.designer.util.ResourceUtil.getIconManager();

		static protected ImageIcon rootIcon;
		static protected ImageIcon projectIcon;
		static protected ImageIcon workflowIcon;

		static protected ImageIcon expandedIcon;

		static {
			try
			{
				rootIcon = (ImageIcon)iconManager.getIcon("frameIcon.gif");
				expandedIcon = (ImageIcon)iconManager.getIcon("workflow.gif");
				workflowIcon = (ImageIcon)iconManager.getIcon("workflow.gif");
				projectIcon = (ImageIcon)iconManager.getIcon("project.gif");
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}

		public Component getTreeCellRendererComponent(
			JTree tree,
			Object value,
			boolean selected,
			boolean expanded,
			boolean leaf,
			int row,
			boolean hasFocus)
		{
			OBETreeData userObject = (OBETreeData) ((OBETreeNode) (value)).getUserObject();
			String stringValue = tree.convertValueToText(userObject.getName(), selected, expanded, leaf, row, hasFocus);
			String iconName = userObject.getIconName();
			setText(stringValue);
			setToolTipText(stringValue);

			String type = userObject.getType();
			if (type.equals(OBETreeNode.ROOT))
			{
				setIcon(rootIcon);
			}
			else if (type.equals(OBETreeNode.PROJECT))
			{
				setIcon(projectIcon);
			}
			else if (type.equals(OBETreeNode.MODULE))
			{
				setIcon(workflowIcon);
			}

			if (selected)
				setForeground(Color.red);
			else
				setForeground(null);

			return this;
		}
	}

}