/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * Thrown in response to an error during retrieving DNS records
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ResolutionException extends Exception {

	protected final static long serialVersionUID = 7890123789234L;

	public ResolutionException ( ) {
		super( "An exception occured while attempting a DNS lookup" );
	}

	public ResolutionException ( String msg ) {
		super( "The following problem was encountered while retrieving a DNS record: " + msg );
	}

}
