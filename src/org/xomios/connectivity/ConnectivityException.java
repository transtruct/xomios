/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

import org.xomios.IOException;

/**
 * Thrown when an error occurs related to the connection of an input/output
 * system.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ConnectivityException extends IOException {

	private static final long serialVersionUID = 1848558316149299861L;

	public ConnectivityException ( String message ) {
		super( message );
	}

}
