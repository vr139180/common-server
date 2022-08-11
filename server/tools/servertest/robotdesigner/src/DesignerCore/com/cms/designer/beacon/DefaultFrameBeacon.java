package com.cms.designer.beacon;

import javax.swing.AbstractButton;

import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.ConstValue;

/**
 * @author excenon
 * $Id: DefaultFrameBeacon.java,v 1.1 2004/06/30 06:36:48 jeffery Exp $
 */
public class DefaultFrameBeacon extends EventObservable
{	
	//-1:use default action,0:disable,1:enable
	private static String[] menuArray={/*
		ConstValue.CoreFileNew,			"-1",
		ConstValue.CoreFileOpen,		"-1",*/
		ConstValue.CoreFileClose,		"0",
		ConstValue.CoreFileSave,		"0",
		ConstValue.CoreFilePrint,		"0",
		ConstValue.CoreEditCut,			"0",
		ConstValue.CoreEditCopy,		"0",
		ConstValue.CoreEditPaste,		"0",
		ConstValue.CoreEditSelectAll,	"0",
		ConstValue.CoreEditSelectNone,	"0",
		ConstValue.CoreViewZoomin,		"0",
		ConstValue.CoreViewZoomout,		"0",
		ConstValue.CoreWinCascade,		"0",
		ConstValue.CoreWinHorizontal,	"0",
		ConstValue.CoreWinVertical,		"0",
		ConstValue.CoreWinClose,		"0"};
	private static String[] buttonArray={/*
		ConstValue.CoreFileNew,			"-1",
		ConstValue.CoreFileOpen,		"-1",*/
		ConstValue.CoreFileSave,		"0",
		ConstValue.CoreEditCut,			"0",
		ConstValue.CoreEditCopy,		"0",
		ConstValue.CoreEditPaste,		"0",
		ConstValue.CoreViewZoomin,		"0",
		ConstValue.CoreViewZoomout,		"0"};

	public DefaultFrameBeacon(OBEDesigner parent){
		super(new EventObserver(parent));

		int i=-1;
		String name;
		int status;
		AbstractButton button;
		for(i=0;i<menuArray.length;i++){
			name=menuArray[i];
			status=Integer.parseInt(menuArray[++i]);
			button=parent.getOBEMenuBar().getByName(name);
			buttonList.add(new ButtonStatus(name,button,status));
		}
		for(i=0;i<buttonArray.length;i++){
			name=buttonArray[i];
			status=Integer.parseInt(buttonArray[++i]);
			button=parent.getButtonManager().getByName(name);
			buttonList.add(new ButtonStatus(name,button,status));
		}
	}
}
