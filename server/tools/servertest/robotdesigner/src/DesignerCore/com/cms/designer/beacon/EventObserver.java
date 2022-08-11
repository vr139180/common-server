package com.cms.designer.beacon;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

import javax.swing.AbstractButton;

import com.cms.designer.swingui.OBEDesigner;

/**
 * @author excenon $Id: EventObserver.java,v 1.1.2.4 2004/06/18 07:58:12 gaj
 *         Exp $
 */
public class EventObserver implements Observer
{
	private OBEDesigner parent;

	public EventObserver( OBEDesigner parent)
	{
		this.parent = parent;
	}

	public void update( Observable obj, Object message)
	{
		List buttonList = (ArrayList) message;
		ButtonStatus oneButtonStatus;
		String name;
		AbstractButton button;
		int status;

		int i = -1;
		for( i = 0; i < buttonList.size(); i++)
		{
			oneButtonStatus = (ButtonStatus) buttonList.get( i);
			name = oneButtonStatus.getName();
			button = oneButtonStatus.getButton();
			status = oneButtonStatus.getStatus();
			if( button == null)
				continue;
			if( status == 0 || status == 1)
				button.setEnabled( getBooleanValue( status));
		}
	}

	private boolean getBooleanValue( int value)
	{
		boolean enable = false;

		switch( value)
		{
		case 0:
			enable = false;
			break;
		case 1:
			enable = true;
			break;
		}
		return enable;
	}
}
