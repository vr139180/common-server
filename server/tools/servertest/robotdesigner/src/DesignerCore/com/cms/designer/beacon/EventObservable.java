package com.cms.designer.beacon;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

/**
 * @author excenon $Id: EventObservable.java,v 1.1.2.2 2004/06/17 08:18:24 gaj
 *         Exp $ $Id: EventObservable.java,v 1.1.2.2 2004/06/17 08:18:24 gaj
 *         Exp $
 */
public class EventObservable extends Observable
{
	protected List buttonList = new ArrayList();

	public EventObservable( Object obj)
	{

		if( obj == null)
		{
		}
		else if( obj instanceof Observer)
			addObserver( (Observer) obj);
	}

	public void sendMessage()
	{
		setChanged();
		notifyObservers( buttonList);
	}

	/**
	 * @return Returns the buttonList.
	 */
	public List getButtonList()
	{
		return buttonList;
	}
}
