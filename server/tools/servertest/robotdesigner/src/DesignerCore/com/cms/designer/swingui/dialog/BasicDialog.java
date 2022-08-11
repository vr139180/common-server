package com.cms.designer.swingui.dialog;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.util.Utilities;

public abstract class BasicDialog extends JDialog
{

	public static final int APPROVE_OPTION = 1;

	public static final int CANCEL_OPTION = 2;

	private int result = CANCEL_OPTION;

	public static final int TYPE_TWO = 102;

	public static final int TYPE_FOUR = 104;

	private int type = TYPE_FOUR;

	protected JButton okButton;

	protected JButton cancelButton;

	protected JButton applyButton;

	protected JButton helpButton;

	protected JTabbedPane tabbedPane;
	
	protected Frame owner;

	public BasicDialog( Frame owner, boolean model, int type)
	{
		super( owner, model);
		this.type = type;
		initComponent();
	}

	public BasicDialog( Frame owner, boolean model, int type, boolean init)
	{
		super( owner, model);
		this.type = type;
		if( init)
		{
			initComponent();
		}
	}

	public abstract boolean save();

	protected void initComponent()
	{
		tabbedPane = new JTabbedPane();
		addTabPanel();
		getContentPane().setLayout( new BorderLayout());
		getContentPane().add( tabbedPane, BorderLayout.CENTER);
		getContentPane().add( createButtonPanel(), BorderLayout.SOUTH);
		getRootPane().setDefaultButton( okButton);
		setModal( true);

		pack();
	}

	abstract protected void addTabPanel();

	protected void addPane( String paneTitle, JPanel pane)
	{
		tabbedPane.add( paneTitle, pane);
	}

	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		okButton = new JButton( ResourceUtil.getRS( "detail.ok"));
		okButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				if( save())
				{
					result = APPROVE_OPTION;
					close();
				}
			}
		});
		panel.add( okButton);

		cancelButton = new JButton( ResourceUtil.getRS( "detail.cancel"));
		cancelButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				result = CANCEL_OPTION;
				close();
			}
		});
		panel.add( cancelButton);

		if( type == BasicDialog.TYPE_FOUR)
		{
			/*
			applyButton = new JButton( ResourceUtil.getRS( "detail.application"));
			applyButton.addActionListener( new ActionListener()
			{
				public void actionPerformed( ActionEvent evt)
				{
					save();
				}
			});
			panel.add( applyButton);

			helpButton = new JButton( ResourceUtil.getRS( "detail.help"));
			helpButton.addActionListener( new ActionListener()
			{
				public void actionPerformed( ActionEvent evt)
				{
					close();
				}
			});
			panel.add( helpButton);
			*/
		}

		return panel;
	}

	public int showDialog()
	{
		Utilities.center( this);
		setVisible( true);
		return result;
	}

	public void close()
	{
		dispose();
	}
}
