package com.cms.designer.swingui.menu;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;

import com.cms.core.util.ResourceUtil;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.EditMode;

public class ModeMenu extends JMenu
{
	
	/**	Construct a new ModeMenu.
	
		@param parent The OBEDesigner
	*/
	public ModeMenu(OBEDesigner parent)
	{
		this.parent = parent;
		init();
	}

	/**	Load the menu's resources. */

	public void loadResources()
	{
		viewModeMenuItem.setText(ResourceUtil.getRS("mode.viewMode"));
		editModeMenuItem.setText(ResourceUtil.getRS("mode.editMode"));
		connectModeMenuItem.setText(ResourceUtil.getRS("mode.connectMode"));
	}

	/**	Initialize the user interface. */

	private void init()
	{
		viewModeMenuItem = new JRadioButtonMenuItem();
		viewModeMenuItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				parent.setEditMode(EditMode.VIEW);
			}
		});
		add(viewModeMenuItem);

		editModeMenuItem = new JRadioButtonMenuItem();
		editModeMenuItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				parent.setEditMode(EditMode.EDIT);
			}
		});
		add(editModeMenuItem);

		connectModeMenuItem = new JRadioButtonMenuItem();
		connectModeMenuItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent evt)
			{
				parent.setEditMode(EditMode.CONNECT);
			}
		});
		add(connectModeMenuItem);

		buttonGroup = new ButtonGroup();
		buttonGroup.add(viewModeMenuItem);
		buttonGroup.add(editModeMenuItem);
		buttonGroup.add(connectModeMenuItem);

		setMode(parent.getEditMode());
		loadResources();
	}
	public void setMode(EditMode mode)
	{
		switch (mode.getValue())
		{
			case EditMode.VIEW_TYPE :
				viewModeMenuItem.setSelected(true);
				break;
			case EditMode.EDIT_TYPE :
				editModeMenuItem.setSelected(true);
				break;
			case EditMode.CONNECT_TYPE :
				connectModeMenuItem.setSelected(true);
				break;
		}
	}

	private OBEDesigner parent;

	private JRadioButtonMenuItem viewModeMenuItem;
	private JRadioButtonMenuItem editModeMenuItem;
	private JRadioButtonMenuItem connectModeMenuItem;
	private ButtonGroup buttonGroup;

}
