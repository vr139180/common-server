/*
 * Created on 2004-8-19
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.cms.designer.swingui;

import java.io.RandomAccessFile;
import java.io.File;
import javax.swing.JEditorPane;

public class OBEXpdlView extends JEditorPane
{
	public OBEXpdlView ()
	{
		super();
		
		init();
		
	}
	
	private void init()
	{
		this.setAutoscrolls( true);
		this.setEditable( false);
		this.setContentType( "text/xml;charset=utf-8");
	}
	
	public void showToolTip( File file)
	{	
		if (file.isDirectory()) return;
		try{
			StringBuilder sb =new StringBuilder();
			
			RandomAccessFile readfile = new RandomAccessFile(file,"r");
			if (readfile.length()<=0) return;
			
			while (true)
			{
				String read = readfile.readLine();
				if (read==null) break;
				sb.append( read);
				sb.append( System.getProperty( "line.separator"));
			}
			
			this.setText( sb.toString());
			this.updateUI();
		}catch(Exception ex){
			ex.printStackTrace();
			
		}
	}
}

