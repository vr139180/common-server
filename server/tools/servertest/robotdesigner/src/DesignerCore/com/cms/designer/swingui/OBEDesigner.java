package com.cms.designer.swingui;

import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.DisplayMode;
import java.awt.Font;
import java.awt.GraphicsDevice;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.ResourceBundle;
import java.util.Vector;
import java.util.prefs.PreferenceChangeEvent;
import java.util.prefs.PreferenceChangeListener;
import java.util.prefs.Preferences;

import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.anthonyeden.lib.gui.StandardAction;
import com.anthonyeden.lib.gui.StatusPanel;
import com.anthonyeden.lib.util.IconManager;
import com.cms.core.util.ResourceUtil;
import com.cms.designer.action.EditCopy;
import com.cms.designer.action.EditCut;
import com.cms.designer.action.EditPaste;
import com.cms.designer.action.EditSelectAll;
import com.cms.designer.action.EditSelectNone;
import com.cms.designer.action.FileClose;
import com.cms.designer.action.FileNew;
import com.cms.designer.action.FileOpen;
import com.cms.designer.action.FilePrint;
import com.cms.designer.action.FileRevert;
import com.cms.designer.action.FileSave;
import com.cms.designer.action.FileSaveas;
import com.cms.designer.action.HelpAbout;
import com.cms.designer.action.HelpTopic;
import com.cms.designer.action.NewProject;
import com.cms.designer.action.ProjectDelete;
import com.cms.designer.action.ProjectExport;
import com.cms.designer.action.ProjectImport;
import com.cms.designer.action.SystemQuit;
import com.cms.designer.coremodule.ModuleManager;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.coremodule.OBEModuleStandard;
import com.cms.designer.coremodule.workspace.ElementKey;
import com.cms.designer.manager.ButtonManager;
import com.cms.designer.manager.ComponentBarManager;
import com.cms.designer.manager.PropertyNames;
import com.cms.designer.manager.ToolbarManager;
import com.cms.designer.manager.WorkSpaceManager;
import com.cms.designer.swingui.ui.DocingPanelManager;
import com.cms.designer.util.ConstValue;
import com.cms.designer.util.EditMode;
import com.cms.designer.util.Scale;
import com.cms.designer.validate.Validate;
import com.cms.designer.workflow.WorkFlowMenuManager;
import com.cms.designer.workflow.action.NewWorkflowProcess;

/**
 * @author jeffery
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class OBEDesigner extends JFrame	implements PreferenceChangeListener, PropertyNames
{
	//用到的资源
	public static ResourceBundle res =null;

	//工作目录
	private static String idepath =System.getProperty("ide.home");
	private static File workingDirectory =new File( idepath);

	//日子管理
	private static Log log = LogFactory.getLog(OBEDesigner.class);
	//图片资源
	public static IconManager iconManager = com.cms.designer.util.ResourceUtil.getIconManager();

	//鼠标资源
	public static Cursor cursor =null;
	{
		cursor =Toolkit.getDefaultToolkit().createCustomCursor(
			((ImageIcon)iconManager.getIcon("cursor.gif")).getImage(),
			new java.awt.Point(0, 0), "");
	}
	
	//显示模式
	private static DisplayMode[] BEST_DISPLAY_MODES =
		new DisplayMode[] {
			new DisplayMode(640, 480, 32, 0),
			new DisplayMode(640, 480, 16, 0),
			new DisplayMode(640, 480, 8, 0)};

	private static int dividerSize =0;

	//个性化设置
	private Preferences preferences;

	private Map actions =null;

	private File currentFile =null;

	//splitter窗口，分割文本区和项目管理区
	public JSplitPane tabSplit;
	//项目管理树
	private OBETree tree;
	//xpdl预览
	private OBEXpdlView xpdlview;
	//错误信息提示栏
	private OBECheckPanel checkPanel;

	//状态条
	private StatusPanel statusPanel;
	//菜单栏
	private OBEMenuBar menuBar;

	//项目管理树
	private OBETabBar tabBar;
	
	//internalframe 管理
	private OBEDesktopPane desktopPane;
	
	//浮动工具条管理
	private DocingPanelManager docingPanelManager;
	//按钮管理
	private ButtonManager buttonManager;
	//工具条管理
	private ToolbarManager toolbarManager;
	//附加工具条管理
	private ComponentBarManager componentbarManager;
	private WorkSpaceManager workSpaceManager;

	private ButtonGroup checkMenuGroup;
	private Hashtable dataHashtable;

	//TODO:编辑模式
	private EditMode editMode = EditMode.EDIT;

	//用户列表和部门列表
	private Vector userList = new Vector();
	private Vector partyList = new Vector();
	
	//监测信息监听器
	private List checkListener =new ArrayList();

	public OBEDesigner()
	{
		preferences = Preferences.userNodeForPackage(OBEDesigner.class);
		preferences.addPreferenceChangeListener(this);
		preferences.node("appearance").addPreferenceChangeListener(this);
		
		updateLookAndFeel();

		//初始化
		OBEModuleManager.getInstance();
		OBEModuleManager.getInstance().initModules( this);

		//初始化界面
		init();
		//加载项目信息
		workSpaceManager.loadWorkProjects();
		//刷新信息树
		workSpaceManager.refrushWorkspaceTree();
	}

	public void addCheckListener( CheckToolTip tt)
	{
		if( checkListener.contains( tt))
			return;
		checkListener.add( tt);
	}
	
	public void removeCheckListener( CheckToolTip tt)
	{
		checkListener.remove( tt);
	}
	
	public synchronized void fireCheckToolTipInfo( boolean remind, Validate validate)
	{
		Iterator ff =checkListener.iterator();
		while( ff.hasNext())
		{
			CheckToolTip tt =( CheckToolTip)ff.next();
			tt.toolTipInfo( remind, validate);
		}
	}

	/*
	 * 设置显示模式
	 */
	private static DisplayMode getBestDisplayMode(GraphicsDevice device)
	{
		for (int x = 0; x < BEST_DISPLAY_MODES.length; x++)
		{
			DisplayMode[] modes = device.getDisplayModes();
			for (int i = 0; i < modes.length; i++)
			{
				if (modes[i].getWidth() == BEST_DISPLAY_MODES[x].getWidth()
					&& modes[i].getHeight() == BEST_DISPLAY_MODES[x].getHeight()
					&& modes[i].getBitDepth()
						== BEST_DISPLAY_MODES[x].getBitDepth())
				{
					return BEST_DISPLAY_MODES[x];
				}
			}
		}
		return null;
	}

	public static void chooseBestDisplayMode(GraphicsDevice device)
	{
		DisplayMode best = getBestDisplayMode(device);
		if (best != null)
		{
			device.setDisplayMode(best);
		}
	}

	/**	Get the current working directory.  The initial value is the value
	 of the system property <code>ide.home</code>.
	 @return The current working directory
	 */
	public static File getWorkingDirectory()
	{
		return workingDirectory;
	}

	public void exitClient()
	{
		saveApplicationPreferences();
		System.exit(0);
	}

	public EditMode getEditMode()
	{
		return editMode;
	}

	public void setEditMode(EditMode editMode)
	{
		if (editMode == null)
			throw new IllegalArgumentException("Edit mode cannot be null");

		JInternalFrame[] internalFrames = desktopPane.getAllFrames();
		/*
		if (editMode == EditMode.VIEW)
		{
			for (int i = 0; i < internalFrames.length; i++)
			{
				OBEInternalFrame frame = (OBEInternalFrame)internalFrames[i];
				OBEGraph graph = (OBEGraph)frame.getGraph();
				graph.setMoveable(false);
				graph.setEditable(false);
			}
		}
		else if (editMode == EditMode.EDIT)
		{
			for (int i = 0; i < internalFrames.length; i++)
			{
				OBEInternalFrame frame = (OBEInternalFrame)internalFrames[i];
				OBEGraph graph = (OBEGraph)frame.getGraph();
				graph.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
				graph.setMoveable(true);
				graph.setEditable(true);
			}
		}
		else if (editMode == EditMode.CONNECT)
		{
			for (int i = 0; i < internalFrames.length; i++)
			{
				OBEInternalFrame frame = (OBEInternalFrame)internalFrames[i];
				OBEGraph graph = (OBEGraph)frame.getGraph();
				graph.setMoveable(false);
				graph.setEditable(true);
			}
		}
		*/

		this.editMode = editMode;
	}

	/*
	 * 获取窗口特征
	 */
	public Preferences getPreferences()
	{
		return preferences;
	}

	/* (non-Javadoc)
	 * @see java.util.prefs.PreferenceChangeListener#preferenceChange(java.util.prefs.PreferenceChangeEvent)
	 */
	public void preferenceChange(PreferenceChangeEvent evt)
	{
		try
		{
			Preferences node = evt.getNode();
			applyPreferences();
			updateLookAndFeel();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * 增加属性
	 */
	public void applyPreferences()
	{
		int size = desktopPane.getAllFrames().length;
		for (int i = 0; i < size; i++)
		{
			OBEInternalFrame frame =
				(OBEInternalFrame)desktopPane.getAllFrames()[i];
			/*
			OBEGraph graph = (OBEGraph)frame.getGraph();
			graph.setAntiAliased(
				preferences.node("appearance").getBoolean("anti-alias", false));
			*/
		}
	}
	
	/**
	 * 设置外观
	 */
	public void updateLookAndFeel()
	{
		try
		{
			UIManager.setLookAndFeel(
				preferences.node("appearance").get(
					"look-and-feel", UIManager.getSystemLookAndFeelClassName()));
			
			Font font = new Font("Dialog",Font.PLAIN,12);
			UIManager.put("ToolTip.font",font);
			UIManager.put("Table.font",font);
			UIManager.put("TableHeader.font",font); 
			UIManager.put("TextField.font",font); 
			UIManager.put("ComboBox.font",font); 
			UIManager.put("PasswordField.font",font); 
			UIManager.put("TextArea.font",font); 
			UIManager.put("TextPane.font",font); 
			UIManager.put("EditorPane.font",font); 
			UIManager.put("FormattedTextField.font",font); 
			UIManager.put("Button.font",font); 
			UIManager.put("CheckBox.font",font); 
			UIManager.put("RadioButton.font",font); 
			UIManager.put("ToggleButton.font",font); 
			UIManager.put("ProgressBar.font",font); 
			UIManager.put("DesktopIcon.font",font); 
			UIManager.put("TitledBorder.font",font); 
			//UIManager.put("Label.font",font); 
			UIManager.put("List.font",font); 
			UIManager.put("TabbedPane.font",font); 
			UIManager.put("MenuBar.font",font); 
			UIManager.put("Menu.font",font); 
			UIManager.put("MenuItem.font",font); 
			UIManager.put("PopupMenu.font",font); 
			UIManager.put("CheckBoxMenuItem.font",font); 
			UIManager.put("RadioButtonMenuItem.font",font); 
			UIManager.put("Spinner.font",font); 
			UIManager.put("Tree.font",font); 
			UIManager.put("ToolBar.font",font); 
			UIManager.put("OptionPane.messageFont",font); 
			UIManager.put("OptionPane.buttonFont",font);
			
			SwingUtilities.updateComponentTreeUI(this);

			if (this.getOBEMenuBar() != null)
				SwingUtilities.updateComponentTreeUI(this.getOBEMenuBar());
		}
		catch (Exception e)
		{
			e.printStackTrace();
			log.error("Error changing look and feel: " + e.getMessage());
		}
	}

	public void setScale(Scale scale)
	{
		/*
		OBEGraph graph = (OBEGraph)this.getCurrentGraph();
		if (graph != null)
		{
			graph.setScale(scale.getValue());
		}
		*/
	}

	/**
	 * 导入应用的特征
	 */
	protected void loadApplicationPreferences()
	{
		Preferences appNode = preferences.node("app");
		Preferences windowNode = preferences.node("window");
		if (windowNode.getBoolean("remember", false))
		{
			setBounds(
				new Rectangle(
					windowNode.getInt("x", 20),
					windowNode.getInt("y", 20),
					windowNode.getInt("width", 400),
					windowNode.getInt("height", 300)));
		}
	}

	/*
	 * 保存应用特征
	 */
	protected void saveApplicationPreferences()
	{
		Preferences appNode = preferences.node("app");
		if ( currentFile != null)
			appNode.put("lastFile", currentFile.getAbsolutePath());
		else
			appNode.remove("lastFile");

		Preferences windowNode = preferences.node("window");
		Rectangle bounds = getBounds();
		windowNode.putInt("x", bounds.x);
		windowNode.putInt("y", bounds.y);
		windowNode.putInt("width", bounds.width);
		windowNode.putInt("height", bounds.height);
		windowNode.putBoolean("remember", true);
	}

	public void setCurrentFile(File currentFile)
	{
	}

	/**
	 * 初始化框架界面
	 */
	private void init()
	{
		//设置IDE图标
		this.setFrameTitle( null);
		
		ImageIcon frameIcon = (ImageIcon)iconManager.getIcon("frameIcon.gif");
		setIconImage(frameIcon.getImage());

		//框架排布器
		getContentPane().setLayout(new BorderLayout());

		dataHashtable = new Hashtable();
		
		//初始化动作事件
		initActions();
		//初始化工具条用到的按钮
		buttonManager = new ButtonManager(this);

		//-------------------------------------------//
		//初始化主菜单
		menuBar = new OBEMenuBar(this);
		setJMenuBar(menuBar);

		//设置中心区域
		JPanel designerPanel = new JPanel(new BorderLayout());
		getContentPane().add( designerPanel, BorderLayout.CENTER);
		
		//工具条拖动管理
		docingPanelManager =new DocingPanelManager( this, designerPanel);
		
		//设置工具条
		//分离通用的工具条按钮和模块工具条按钮
		toolbarManager = new ToolbarManager(this);
		docingPanelManager.addOneToolBar( toolbarManager.getToolBar(), BorderLayout.NORTH);

		//中心控制区域
		JPanel centerPanel = new JPanel();
		centerPanel.setLayout( new BorderLayout());
		designerPanel.add( centerPanel, BorderLayout.CENTER);

		componentbarManager = new ComponentBarManager(this);

		//分割窗口包含internalframe和项目管理窗口
		//树中的tab管理
		xpdlview = new OBEXpdlView();
		tree = new OBETree(this);
		checkPanel =new OBECheckPanel();
		workSpaceManager = new WorkSpaceManager(this);
		tabBar = new OBETabBar(this);
		
		//浏览
		
		
		//frame窗口
		desktopPane = new OBEDesktopPane(this);
		desktopPane.setOpaque(true);
		desktopPane.setDragMode(1);

		//放入split中
		tabSplit =new JSplitPane( JSplitPane.HORIZONTAL_SPLIT, desktopPane, tabBar);
		centerPanel.add(tabSplit, BorderLayout.CENTER);

		//修改tabsplit的属性
		dividerSize = tabSplit.getDividerSize();
		tabSplit.setDividerLocation(650);

		//设置状态条
		statusPanel = new StatusPanel();
		getContentPane().add( statusPanel, BorderLayout.SOUTH);

		checkMenuGroup = new ButtonGroup();

		pack();

		loadApplicationPreferences();
		applyPreferences();

		//关闭应用的事件监听器
		addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent evt){
				exitClient();
			}
		});
	}

	/**
	 * 设置frame的标题
	 * @param title
	 */
	public void setFrameTitle(String title)
	{
		StringBuffer buffer = new StringBuffer();
		buffer.append(ResourceUtil.getRS("window.caption"));
		if( title != null)
		{
			buffer.append( ":(");
			buffer.append( title);
			buffer.append( ")");
		}
		
		setTitle(buffer.toString());
	}

	/**
	 * 菜单事件初始化
	 */
	private void initActions()
	{
		if( actions == null)
			actions = new HashMap();
		
		//系统基本操作
		//文件操作
		actions.put( ConstValue.CoreFileNew, new FileNew( this));
		actions.put( ConstValue.CoreFileOpen, new FileOpen( this));
		actions.put( ConstValue.CoreFileClose, new FileClose( this));
		actions.put( ConstValue.CoreFileRevert, new FileRevert( this));
		actions.put( ConstValue.CoreFileSave, new FileSave( this));
		actions.put( ConstValue.CoreFileSaveas, new FileSaveas( this));
		actions.put( ConstValue.CoreFilePrint, new FilePrint( this));
		actions.put( ConstValue.CoreSysQuit, new SystemQuit( this));

		//编辑操作
		actions.put( ConstValue.CoreEditCut, new EditCut(this));
		actions.put( ConstValue.CoreEditCopy, new EditCopy(this));
		actions.put( ConstValue.CoreEditPaste, new EditPaste(this));
		//TODO:actions.put( ConstValue.CoreEditRedo, new EditRedo(this));
		//TODO:actions.put( ConstValue.CoreEditUndo, new EditUndo(this));

		actions.put( ConstValue.CoreEditSelectAll, new EditSelectAll(this));
		actions.put( ConstValue.CoreEditSelectNone, new EditSelectNone(this));
		
		//TODO:
		actions.put( "edit.preferences",
			new StandardAction(this, "editPreferences"));
		
		//视图菜单
		actions.put(ConstValue.CoreViewZoomin, new StandardAction(this, ConstValue.CoreViewZoomin));
		actions.put(ConstValue.CoreViewZoomout, new StandardAction(this, ConstValue.CoreViewZoomout));

		actions.put( "format.textFont",
			new StandardAction(this, "formatTextFont"));
		actions.put( "internalFrame.revert",
			new StandardAction(this, "revertInternalFrame"));
		actions.put( "internalFrame.close",
			new StandardAction(this, "closeInternalFrame"));
		actions.put( "internalFrame.minimize",
			new StandardAction(this, "minimizeInternalFrame"));

		//帮助菜单
		actions.put(ConstValue.CoreHelpTopic, new HelpTopic(this));
		actions.put(ConstValue.CoreHelpAbout, new HelpAbout(this));
		
		//工程管理事件
		actions.put( ConstValue.ProjectNew, new NewProject(this));
		actions.put( ConstValue.ProjectImport, new ProjectImport(this));
		actions.put( ConstValue.ProjectExport, new ProjectExport(this));
		actions.put( ConstValue.ProjectDelete, new ProjectDelete(this));
		actions.put( WorkFlowMenuManager.ACT_NEWPROCESS, new NewWorkflowProcess( this));
	}

	/**
	 * 根据不通的模式创建split
	 * @param i
	 */
	public void createSplitPane(int i)
	{
		tabSplit.remove( desktopPane);
		tabSplit.remove( tabBar);

		tabSplit.setDividerSize(0);
		if (i == SwingConstants.WEST)
		{
			tabSplit.setOrientation(JSplitPane.HORIZONTAL_SPLIT);
			tabSplit.setRightComponent( desktopPane);
			tabSplit.setLeftComponent(tabBar);
			
			tabSplit.setDividerSize(dividerSize);
			tabSplit.setDividerLocation(200);
		}
		else if (i == SwingConstants.NORTH)
		{
			tabSplit.setOrientation(JSplitPane.VERTICAL_SPLIT);
			tabSplit.setBottomComponent( desktopPane);
			tabSplit.setTopComponent(tabBar);
			
			tabSplit.setDividerSize(dividerSize);
			tabSplit.setDividerLocation(280);
		}
		else if (i == SwingConstants.EAST)
		{
			tabSplit.setOrientation(JSplitPane.HORIZONTAL_SPLIT);
			tabSplit.setLeftComponent( desktopPane);
			tabSplit.setRightComponent(tabBar);
			
			tabSplit.setDividerSize(dividerSize);
			tabSplit.setDividerLocation(600);
		}
		else if (i == SwingConstants.SOUTH)
		{
			tabSplit.setOrientation(JSplitPane.VERTICAL_SPLIT);
			tabSplit.setTopComponent( desktopPane);
			tabSplit.setBottomComponent(tabBar);
			
			tabSplit.setDividerSize(dividerSize);
			tabSplit.setDividerLocation(350);
		}
	}

	/*
	 * TODO:用来保存存在的internalframe菜单项，window menu里面使用
	 */
	public ButtonGroup getCheckMenuGroup()
	{
		return this.checkMenuGroup;
	}

	/**
	 * split左边控件的宽度
	 * @return
	 */
	public int splitLeftPanelWidth()
	{
		return tabSplit.getLeftComponent().getWidth();
	}

	/**
	 * split左边控件的高度
	 * @return
	 */
	public int splitLeftPanelHeight()
	{
		return tabSplit.getLeftComponent().getHeight();
	}

	/**
	 * 获取internalframe的个数
	 * @return
	 */
	public int vectorCount()
	{
		return desktopPane.getComponentCount();
	}

	/**
	 * 切换视图时
	 * @param name
	 */
	public void changeView(String name)
	{
		//视图切换时变换TOOLBAR
		ElementKey ek =new ElementKey();
		ek.analyze( name);
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =mm.getModule( ek.getModuleID());
		if( ms == null)
			return;
		if( ms.getModuleToolbar() != null)
			docingPanelManager.addOneToolBar( ms.getModuleToolbar(), BorderLayout.WEST);

		docingPanelManager.updateUI();
		//更新系统工具条事件
		toolbarManager.updateToolbarActions( ms.getIDEAction());
		toolbarManager.updateMenubarActions( ms.getIDEAction());
	}
	
	/**
	 * 视图失去焦点的时候
	 * @param name
	 */
	public void viewDeactivated( String name)
	{
		ElementKey ek =new ElementKey();
		ek.analyze( name);
		ModuleManager mm =OBEModuleManager.getInstance().getModuleManager();
		OBEModuleStandard ms =mm.getModule( ek.getModuleID());
		if( ms == null)
			return;
		docingPanelManager.removeOneToolBar( ms.getModuleToolbar());

		docingPanelManager.updateUI();
	}

	//--------------------------------------------//
	//--------------------------------------------//
	//--------------------------------------------//
	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.IOBEMainFrame#getToolBar()
	 */
	public OBEToolBar getToolBar()
	{
		return toolbarManager.getToolBar();
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.IOBEMainFrame#getComponentbarManager()
	 */
	public ComponentBarManager getComponentbarManager()
	{
		return componentbarManager;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.IOBEMainFrame#getActions()
	 */
	public Map getActions()
	{
		return actions;
	}

	/* (non-Javadoc)
	 * @see com.excenon.designer.coremodule.IOBEMainFrame#getOBEMenuBar()
	 */
	public OBEMenuBar getOBEMenuBar()
	{
		return this.menuBar;
	}

	public ButtonManager getButtonManager()
	{
		return buttonManager;
	}

	public ToolbarManager getToolbarManager()
	{
		return toolbarManager;
	}

	public DocingPanelManager getDocingPanelManager()
	{
		return docingPanelManager;
	}

	public OBEDesktopPane getDesktopPanel()
	{
		return desktopPane;
	}

	public OBETree getTree()
	{
		return tree;
	}
	
	public OBEXpdlView getXpdlView(){
		return xpdlview;
	}
	
	public OBECheckPanel getCheckPanel()
	{
		return checkPanel;
	}

	public JSplitPane getTabSplit()
	{
		return tabSplit;
	}

	public void setStatus(String status)
	{
		statusPanel.setStatus(status);
	}

	public void setStatusVisiable( boolean b)
	{
		statusPanel.setVisible( b);
	}
	
	public void setTabBarVisiable( boolean b)
	{
		tabBar.setVisible( b);
	}
	
	public WorkSpaceManager getWorkSpaceManager()
	{
		return workSpaceManager;
	}

	public Hashtable getHashtable()
	{
		return dataHashtable;
	}

	/**
	 * 返回工作组列表
	 * @return
	 */
	public Vector getPartyList()
	{
		//同时从数据库中更新
		partyList.clear();
		
			
		return partyList;
	}
	
	/**
	 * 返回用户列表
	 * @return
	 */
	public Vector getUserList()
	{
		userList.clear();

			
		return userList;
	}
	/**
	 * 返回用户列表2  用户包括 选定部门下的用户
	 * @return
	 */
	public Vector getUser2List()
	{
		userList.clear();
		
		return userList;
	}
	
}