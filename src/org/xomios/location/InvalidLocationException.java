/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.location;

import org.xomios.IOException;

/**
 * Thrown when an attempt is made to access a location that does not exist or
 * is not currently available.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InvalidLocationException extends IOException {

	private static final long serialVersionUID = -6255999560493546154L;

	public InvalidLocationException ( String message ) {
		super( message );
	}

}
