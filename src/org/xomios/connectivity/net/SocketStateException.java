/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * Thrown in response to a action being requested on a socket that is
 * inconsistent with its current state or configuration
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class SocketStateException extends RuntimeException {

	protected static final long serialVersionUID = 378490359091234L;

	public SocketStateException ( ) {
		super( "The underlying Socket object was in a state inconsistent with the operation requested on it" );
	}

	public SocketStateException ( String msg ) {
		super( msg );
	}

}
