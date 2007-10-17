/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * 
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InvalidAddressException extends Exception {

	static final long serialVersionUID = 40712345230012L;

	public InvalidAddressException ( ) {
		super( "Invalid IP address in byte array" );
	}

	public InvalidAddressException ( String msg ) {
		super( msg );
	}
}
