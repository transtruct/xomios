/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;


/**
 * Exception thrown when an attempt to manipulate an input/output mechanism is
 * restricted due to insufficient permissions, unavailability, or other
 * circumstances related to obtaining sufficient access to the mechanism.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class AccessException extends IOException {

	private static final long serialVersionUID = 7924995288684860418L;

	public AccessException ( String message ) {
		super( message );
	}

}
