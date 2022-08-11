package com.cms.core.workflow.parser;

/**
 * This interface provides static final constants for all of the XPDL tag names.
 */
public interface XPDLNames {

	/** Unique identifier. */
	public static final String ID = "id";
	
	/** Entity name. */
	public static final String NAME = "n";
	public static final String VALUE = "v";

	public static final String EXTENDED_ATTRIBUTES = "exts";
	public static final String EXTENDED_ATTRIBUTE = "ext";

	public static final String WORKFLOW_PROCESSES = "bhs";
	public static final String WORKFLOW_PROCESS = "bh";

	public static final String ACTIVITY = "s";

	public static final String ACTIONS = "acts";
	public static final String ACTION = "act";
	public static final String ACTION_DONE = "done";
	public static final String ACTIONPM = "pm";
	public static final String ACTIONPM_N = "n";
	public static final String ACTIONPM_V = "v";

	public static final String TRANSITIONS = "tfs";

	public static final String TRANSITION = "tf";

	public static final String FROM = "f";

	public static final String TO = "t";

	public static final String CONDITION = "c";
}
