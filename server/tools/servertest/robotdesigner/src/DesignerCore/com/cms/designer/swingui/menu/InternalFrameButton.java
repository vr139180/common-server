package com.cms.designer.swingui.menu;

import java.awt.Dimension;
import java.awt.Insets;

import javax.swing.Action;
import javax.swing.JButton;

public class InternalFrameButton extends JButton
{
	private static int width = 16;
	private static int height = 13;

	//Œª÷√
	private Dimension dimension;

	public InternalFrameButton()
	{
		super();
		init();
	}

	public InternalFrameButton(Action action)
	{
		super(action);
		init();
	}

	private void init()
	{
		dimension = new Dimension(width, height);
		setPreferredSize(dimension);
		setSize(dimension);
		setMaximumSize(dimension);
		setMinimumSize(dimension);
		setRequestFocusEnabled(false);
		setFocusPainted(false);
		setMargin(new Insets(0, 0, 0, 0));
		setBorder(null);
	}

	public void requestFocus()
	{};

	public boolean isOpaque()
	{
		return true;
	}
}