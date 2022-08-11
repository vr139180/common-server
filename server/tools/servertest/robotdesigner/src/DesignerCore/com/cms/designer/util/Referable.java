package com.cms.designer.util;

public interface Referable
{

	/**
	 * Do save which implement this interface.
	 * 
	 * @return whether the resource is saved
	 */

	public boolean save();

	/**
	 * Do revert which implement this interface.
	 *
	 */

	public void revert();

}
