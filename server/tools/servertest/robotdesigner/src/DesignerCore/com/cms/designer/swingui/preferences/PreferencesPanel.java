package com.cms.designer.swingui.preferences;

import java.util.prefs.Preferences;

public interface PreferencesPanel
{

	/**
	 * Save the panel's preferences.
	 * 
	 * @param preferences
	 *            The Preferences object
	 */

	public boolean save( Preferences preferences);

	/**
	 * Revert the panel's preferences.
	 * 
	 * @param preferences
	 *            The Preferences object
	 */

	public void revert( Preferences preferences);

}
