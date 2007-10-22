/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;

public class IOException extends Exception {

	private static final long serialVersionUID = -252212672057731763L;

	public IOException ( ) {
		super();
	}

	public IOException ( String message ) {
		super( message );
	}

	public IOException ( String message, Throwable cause ) {
		super( message, cause );
	}

	public IOException ( Throwable cause ) {
		super( cause );
	}

}
