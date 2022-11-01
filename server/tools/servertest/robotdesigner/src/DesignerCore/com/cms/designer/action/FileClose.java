package com.cms.designer.action;

import java.awt.event.ActionEvent;

import javax.swing.JOptionPane;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.swingui.OBEInternalFrame;

/**
 * @author Administrator
 * $Id: FileClose.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class FileClose extends OBEStandardAction
{
	private String key =null;
	
	/**
	 * @param parent
	 */
	public FileClose(OBEDesigner parent)
	{
		super(parent);
	}

	public void actionPerformed(ActionEvent e)
	{
		super.actionPerformed(e);
		if(key==null) return;

		OBEInternalFrame frame =( OBEInternalFrame)parent.getDesktopPanel().getInternalFrame( key);
		if( frame == null) return;
		if(frame.isDirty()){
			int flag=-1;
			//todo
			//0:yes;1:no;2:cancel
			flag=JOptionPane.showConfirmDialog(null,"要保存该文件吗?","该文件未保存!",JOptionPane.YES_NO_CANCEL_OPTION);
			if(flag==2) return;
			if(flag==0) return;
		}

		frame.dispose();
	}

	public String getKey()
	{
		return key;
	}

	public void setKey(String key)
	{
		this.key = key;
	}

}
