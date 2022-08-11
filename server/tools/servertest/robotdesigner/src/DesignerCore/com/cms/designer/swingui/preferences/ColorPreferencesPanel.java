package com.cms.designer.swingui.preferences;

import java.awt.BorderLayout;
import java.awt.Color;
import java.util.Iterator;
import java.util.prefs.Preferences;

import javax.swing.JPanel;

import com.anthonyeden.lib.util.XArrayList;
import com.cms.core.util.ResourceUtil;

public class ColorPreferencesPanel extends JPanel implements PreferencesPanel
{
	private XArrayList colors;

	private ColorTableModel colorTableModel;

	private ColorTable colorTable;

	public ColorPreferencesPanel()
	{
		init();
	}

	public boolean save( Preferences preferences)
	{
		Preferences colorPreferences = preferences.node( "color");

		Iterator iter = colors.iterator();
		while( iter.hasNext())
		{
			ColorMap colorMap = (ColorMap) iter.next();

			colorPreferences.put( colorMap.getName(), Integer.toString( colorMap.getColor()
					.getRGB()));
		}

		return true;
	}

	public void revert( Preferences preferences)
	{
		Preferences colorPreferences = preferences.node( "color");
		//antiAliasCheckBox.setSelected(appearancePreferences.getBoolean("anti-alias",
		// false));
	}

	private void init()
	{
		initColors();

		colorTableModel = new ColorTableModel( colors);
		colorTable = new ColorTable( colorTableModel);

		setLayout( new BorderLayout());
		add( colorTable, BorderLayout.CENTER);
	}

	private void initColors()
	{
		colors = new XArrayList();
		colors.add( new ColorMap( ResourceUtil.getRS( "preference.toolSet"), Color.blue));
		colors.add( new ColorMap( ResourceUtil.getRS( "preference.subFlow"), Color.green));
		colors.add( new ColorMap( ResourceUtil.getRS( "preference.loop"), Color.yellow));
	}

}
