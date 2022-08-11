package com.cms.designer.action;

import java.awt.event.ActionEvent;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
public class FilePrint extends OBEStandardAction
{
	public FilePrint(OBEDesigner parent)
	{
		super(parent);
	}

	/**
	 * @see java.awt.event.ActionListener#actionPerformed(ActionEvent)
	 */
	public void actionPerformed(ActionEvent e)
	{
		/*
		PrinterJob printJob = PrinterJob.getPrinterJob();
		printJob.setPrintable(parent.getCurrentGraph());
		if (printJob.printDialog())
		{
			try
			{
				boolean oldvalue = false;
				double oldscale = parent.getCurrentGraph().getScale();
				parent.getCurrentGraph().setScale(1);
				oldvalue = parent.getCurrentGraph().isPageVisible();
				printJob.print();
				parent.getCurrentGraph().setScale(oldscale);
				parent.getCurrentGraph().setPageVisible(oldvalue);
			}
			catch (Exception printException)
			{
				printException.printStackTrace();
			}
		}
		*/
	}
}
