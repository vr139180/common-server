package com.cms.designer.swingui.preferences;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.util.prefs.Preferences;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;

import com.anthonyeden.lib.gui.LookAndFeelListCellRenderer;
import com.cms.core.util.ResourceUtil;

public class AppearancePreferencesPanel extends JPanel implements PreferencesPanel
{
	private JLabel lookAndFeelLabel;

	private JComboBox lookAndFeelComboBox;

	private JCheckBox antiAliasCheckBox;

	public AppearancePreferencesPanel()
	{
		init();
	}

	public boolean save( Preferences preferences)
	{
		Preferences appearancePreferences = preferences.node( "appearance");
		appearancePreferences.putBoolean( "anti-alias", antiAliasCheckBox.isSelected());
		appearancePreferences.put( "look-and-feel",
				((UIManager.LookAndFeelInfo) lookAndFeelComboBox.getSelectedItem()).getClassName());
		return true;
	}

	public void revert( Preferences preferences)
	{
		Preferences appearancePreferences = preferences.node( "appearance");
		antiAliasCheckBox.setSelected( appearancePreferences.getBoolean( "anti-alias", false));
		lookAndFeelComboBox.setSelectedItem( getLookAndFeel( (String) appearancePreferences.get( 
				"look-and-feel", UIManager.getSystemLookAndFeelClassName())));
	}

	private void init()
	{
		GridBagLayout gbl = new GridBagLayout();
		GridBagConstraints gbc = new GridBagConstraints();
		setLayout( gbl);

		gbc.anchor = GridBagConstraints.WEST;
		gbc.fill = GridBagConstraints.HORIZONTAL;

		lookAndFeelLabel = new JLabel( ResourceUtil.getRS( "preference.lookAndFeel"));
		gbc.weightx = 0;
		gbc.gridwidth = 1;
		gbl.setConstraints( lookAndFeelLabel, gbc);
		add( lookAndFeelLabel);

		lookAndFeelComboBox = new JComboBox( UIManager.getInstalledLookAndFeels());
		lookAndFeelComboBox.setRenderer( new LookAndFeelListCellRenderer());
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints( lookAndFeelComboBox, gbc);
		add( lookAndFeelComboBox);

		antiAliasCheckBox = new JCheckBox( ResourceUtil.getRS( "preference.artiAliased"));
		gbc.weightx = 1;
		gbc.gridwidth = GridBagConstraints.REMAINDER;
		gbl.setConstraints( antiAliasCheckBox, gbc);
		add( antiAliasCheckBox);

		JPanel p = new JPanel();
		gbc.weighty = 1;
		gbl.setConstraints( p, gbc);
		add( p);
	}

	private static UIManager.LookAndFeelInfo getLookAndFeel( String className)
	{
		UIManager.LookAndFeelInfo[] lnfs = UIManager.getInstalledLookAndFeels();
		for( int i = 0; i < lnfs.length; i++)
		{
			if( lnfs[i].getClassName().equals( className)){ return lnfs[i]; }
		}
		return null;
	}
}
