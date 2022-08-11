package com.cms.designer.workflow.beacon;

import javax.swing.AbstractButton;

import com.cms.designer.beacon.ButtonStatus;
import com.cms.designer.beacon.EventObservable;
import com.cms.designer.beacon.EventObserver;
import com.cms.designer.swingui.OBEDesigner;
import com.cms.designer.util.ConstValue;

/**
 * @author excenon
 * same behavior(same super class), but different character(different children class) induce different result
 * $Id: OBEWorkFlowFrameBeacon.java,v 1.2 2004/09/02 09:06:18 jefferyd Exp $
 */
public class OBEWorkFlowFrameBeacon extends EventObservable
{
	//-1:default action,0:disable,1:enable
	private static String[] menuArray={
		ConstValue.CoreFileClose,		"1",
		ConstValue.CoreFileSave,		"1",
		ConstValue.CoreFilePrint,		"1",
		ConstValue.CoreEditCut,			"1",
		ConstValue.CoreEditCopy,		"1",
		ConstValue.CoreEditPaste,		"0",
		ConstValue.CoreEditSelectAll,	"1",
		ConstValue.CoreEditSelectNone,	"1",
		ConstValue.CoreViewZoomin,		"1",
		ConstValue.CoreViewZoomout,		"1",
		ConstValue.CoreWinCascade,		"1",
		ConstValue.CoreWinHorizontal,	"1",
		ConstValue.CoreWinVertical,		"1",
		ConstValue.CoreWinClose,		"1"};
	private static String[] buttonArray={
		ConstValue.CoreFileSave,		"1",
		ConstValue.CoreEditCut,			"1",
		ConstValue.CoreEditCopy,		"1",
		ConstValue.CoreEditPaste,		"0",
		ConstValue.CoreViewZoomin,		"1",
		ConstValue.CoreViewZoomout,		"1"};

	public OBEWorkFlowFrameBeacon(OBEDesigner parent){
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
