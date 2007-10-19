/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * Thrown when a incorrectly network address is passed to a method
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class AddressFormatException extends IllegalArgumentException {

	static final long serialVersionUID = 40712345230012L;

	public AddressFormatException ( ) {
		super( "Invalid IP address specified" );
	}

	public AddressFormatException ( String msg ) {
		super( msg );
	}
}
