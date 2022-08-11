package com.cms.designer.swingui.panel;

import java.awt.FlowLayout;

import javax.swing.JPanel;

public class InternalFramePanel extends JPanel
{
	private FlowLayout flowLayout = new FlowLayout();
	private boolean isVisable = true;

	public InternalFramePanel()
	{
		try
		{
			jbInit();
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}

	void jbInit() throws Exception
	{
		flowLayout.setAlignment(FlowLayout.RIGHT);
		flowLayout.setHgap(1);
		this.setLayout(flowLayout);
	}
}