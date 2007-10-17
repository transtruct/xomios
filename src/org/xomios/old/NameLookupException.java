/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.old;

/**
 * Thrown when reverse DNS resolution on a host fails
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class NameLookupException extends Exception {

	static final long serialVersionUID = 23454708347408937L;

	public NameLookupException ( ) {
		super( "Address lookup for host failed" );
	}

	public NameLookupException ( String msg ) {
		super( msg );
	}

}
