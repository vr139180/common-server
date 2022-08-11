package com.cms.designer.swingui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JToolBar;
import javax.swing.Timer;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.MouseInputListener;






import com.cms.core.util.ResourceUtil;
import com.cms.core.workflow.WorkflowPackage;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.designer.coremodule.OBEModuleManager;
import com.cms.designer.swingui.ui.BasicFrameBarUI;
import com.cms.designer.validate.Validate;
import com.cms.designer.workflow.OBEGraphWorkflow;
import com.cms.designer.workflow.OBEWorkFlowFrame;
import com.cms.designer.workflow.OBEWorkflowModule;

public class OBETabBar extends OBEFrameBar implements CheckToolTip
{
	public static final int WORKSPACE_TAB =0;
	public static final int CHECK_TAB =1;
	public static final int VIEW_TAB =2;
	
	private OBEDesigner parent =null;
	private String tabbar = "projectbar";
	private BasicFrameBarUI basicFrameBarUI =null;
	
	private JScrollPane view;
	private OBETree tree;
	private OBECheckPanel checkPanel;
	private OBEXpdlView xpdlview;
	
	private JTabbedPane tabbedPane;
	private int orientation = JToolBar.HORIZONTAL;
	
	//信息提示定时器
	private Timer checkTimer =null;
	
	public OBETabBar( OBEDesigner parent)
	{
		super(parent);
		
		this.parent = parent;
		xpdlview = parent.getXpdlView();
		tree = parent.getTree();
		checkPanel =parent.getCheckPanel();

		init();
		
		//挂接监听器
		parent.addCheckListener( this);
		checkTimer =new Timer( 1200, new ActionListener(){
			public void actionPerformed( ActionEvent e)
			{
				Color old =tabbedPane.getBackgroundAt( CHECK_TAB);
				if( old != null)
					tabbedPane.setBackgroundAt( CHECK_TAB, old.equals( Color.BLUE)? null:Color.BLUE);
				else
					tabbedPane.setBackgroundAt( CHECK_TAB, null);
			}
		});
	}
	
	/**
	 * add treepane or other pages 
	 */
	public void init()
	{
		basicFrameBarUI = new BasicFrameBarUI(parent, tabbar);
		setUI(basicFrameBarUI);
		
		setBorderPainted(true);
		setBorder( BorderFactory.createEmptyBorder(8, 5, 5, 5));
		setRollover(true);
		
		this.setOrientation(1);
		this.setLayout(new BorderLayout());
		
		setMargin(new Insets(20, 5, 0, 0));
		JScrollPane treePane = new JScrollPane(tree);

		tabbedPane = new JTabbedPane();
		tabbedPane.setTabPlacement( JTabbedPane.BOTTOM);
		tabbedPane.add( treePane, ResourceUtil.getRS("window.workplat"), WORKSPACE_TAB);
		
		JScrollPane check =new JScrollPane( checkPanel);
		tabbedPane.add( check, "信息提示", CHECK_TAB);
		
		view =new JScrollPane( xpdlview);
		tabbedPane.add( view, "预览", VIEW_TAB);		

		
		
		add( tabbedPane, BorderLayout.CENTER);
		
		tabbedPane.addChangeListener( new ChangeListener(){
			public void stateChanged( ChangeEvent e)
			{
				if( tabbedPane.getSelectedIndex() == CHECK_TAB)
				{
					tabbedPane.setBackgroundAt( CHECK_TAB, null);
					if( checkTimer.isRunning())
						checkTimer.stop();
				}
				else if( tabbedPane.getSelectedIndex() == VIEW_TAB)
				{
					showXpdl();
				}
				
			}});
		
		MouseInputListener ml = basicFrameBarUI.createdockListener();

		this.addMouseMotionListener(ml);
		this.addMouseListener(ml);
	}
	private void showXpdl(){
		
		xpdlview.setText("");
		
		OBEWorkflowModule module =( OBEWorkflowModule)OBEModuleManager.getInstance().
		getModuleManager().getModule( OBEWorkflowModule.MODULE_ID);
	
		if( module == null)	return ;
		
		Object oframe = parent.getDesktopPanel().getCurrentInternalFrame();
		if ( ! ( oframe instanceof OBEWorkFlowFrame ) ) return;
		
		OBEWorkFlowFrame frame =( OBEWorkFlowFrame)oframe;
		if( frame == null) return ;
		WorkflowProcess process = ((OBEGraphWorkflow)frame.getGraph()).getWorkflowProcess();
		
		WorkflowPackage wp = module.findWP(process);
		if (wp==null) return;
		String filename = wp.getFile().getAbsolutePath();
		
		xpdlview.showToolTip(new File(filename));		
	}
	
	public OBEDesigner getDesignerParent()
	{
		return parent;
	}
	
	public OBETree getTabBarTree()
	{
		return tree;
	}
	
	public int getOrientation()
	{
		return this.orientation;
	}
	
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
	
	public void setFrameBar()
	{
		basicFrameBarUI.setUnFloating();
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

	/* (non-Javadoc)
	 * @see com.excenon.designer.swingui.CheckToolTip#toolTipInfo()
	 */
	public void toolTipInfo( boolean remind, Validate validate)
	{
		checkPanel.showToolTip( validate);
		
		if( tabbedPane.getSelectedIndex() != CHECK_TAB)
		{
			//根据需要决定是否提示
			if( remind )
			{
				if( !checkTimer.isRunning())
					checkTimer.start();
			}
			else
			{
				if( checkTimer.isRunning())
					checkTimer.stop();
			}
		}
	}
}
