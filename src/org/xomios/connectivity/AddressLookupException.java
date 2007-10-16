/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

/**
 * Thrown when a address lookup for a host fails
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class AddressLookupException extends Exception {

	static final long serialVersionUID = 23454708347408937L;

	public AddressLookupException ( ) {
		super( "Address lookup for host failed" );
	}

	public AddressLookupException ( String msg ) {
		super( msg );
	}

}
