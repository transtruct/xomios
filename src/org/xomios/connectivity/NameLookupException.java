/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

/**
 * Thrown when DNS resolution on a host fails
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class NameLookupException extends Exception {

	protected static final long serialVersionUID = 23492034890L;

	/* Possible error values */
	public static final int OTHER_ERROR = -1;
	public static final int HOST_NOT_FOUND = 1;
	public static final int NO_DATA = 2;
	public static final int NO_RECOVERY = 3;
	public static final int TRY_AGAIN = 4;

	protected int error;

	public NameLookupException ( int error ) {
		super( "Error in name lookup" );
		this.error = error;
	}

	public NameLookupException ( String msg, int error ) {
		super( msg );
		this.error = error;
	}

	/**
	 * Get the error associated with this exception. Is one of the static
	 * constants defined in this class
	 * 
	 * @return An integer corresponding to a specific type of name resolution
	 *         error
	 */
	public int getErrorCode ( ) {
		return this.error;
	}

}
