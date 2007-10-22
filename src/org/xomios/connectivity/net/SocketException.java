/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * Thrown when a error occurs during socket operations
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class SocketException extends RuntimeException {

	protected static final long serialVersionUID = 75821340713933L;

	public SocketException ( ) {
		super( "An error was encounter while handling the socket request" );
	}

	public SocketException ( String msg ) {
		super( msg );
	}

}
