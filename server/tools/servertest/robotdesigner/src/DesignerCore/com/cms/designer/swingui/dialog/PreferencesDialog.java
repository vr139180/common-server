package com.cms.designer.swingui.dialog;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.prefs.Preferences;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.preferences.AppearancePreferencesPanel;
import com.cms.designer.swingui.preferences.ColorPreferencesPanel;

public class PreferencesDialog extends JDialog
{
	private OBEDesigner parent;

	private JTabbedPane tabbedPane;

	private JButton saveButton;

	private JButton saveAndCloseButton;

	private JButton closeButton;

	private AppearancePreferencesPanel appearancePanel;

	private ColorPreferencesPanel colorPanel;

	public PreferencesDialog( OBEDesigner parent)
	{
		super( parent, true);
		this.parent = parent;
		init();
	}

	public boolean save()
	{
		boolean saved = true;
		Preferences preferences = parent.getPreferences();

		if( !appearancePanel.save( preferences))
		{
			saved = false;
		}
		if( !colorPanel.save( preferences))
		{
			saved = false;
		}

		return saved;
	}

	public void close()
	{
		dispose();
	}

	private void init()
	{
		appearancePanel = new AppearancePreferencesPanel();
		colorPanel = new ColorPreferencesPanel();

		tabbedPane = new JTabbedPane();
		tabbedPane.add( new JScrollPane( appearancePanel), ResourceUtil
				.getRS( "preference.appearance"));
		tabbedPane.add( new JScrollPane( colorPanel), ResourceUtil.getRS( "preference.colors"));

		getContentPane().setLayout( new BorderLayout());
		getContentPane().add( tabbedPane, BorderLayout.CENTER);
		getContentPane().add( createButtonPanel(), BorderLayout.SOUTH);

		setTitle( ResourceUtil.getRS( "preference.preferences"));

		pack();

		Preferences preferences = parent.getPreferences();

		appearancePanel.revert( preferences);
		colorPanel.revert( preferences);
	}

	private JPanel createButtonPanel()
	{
		JPanel panel = new JPanel();

		saveButton = new JButton( ResourceUtil.getRS( "window.save"));
		saveButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				save();
			}
		});
		panel.add( saveButton);

		saveAndCloseButton = new JButton( ResourceUtil.getRS( "preference.saveAndClose"));
		saveAndCloseButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				if( save())
				{
					close();
				}
			}
		});
		panel.add( saveAndCloseButton);

		closeButton = new JButton( ResourceUtil.getRS( "window.close"));
		closeButton.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent evt)
			{
				close();
			}
		});
		panel.add( closeButton);

		return panel;
	}


}
