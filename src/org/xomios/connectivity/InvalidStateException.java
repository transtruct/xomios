/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;


/**
 * Thrown when an attempt to modify an input/output resource is attempted while
 * the resource is in a state that does permit such a modification.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InvalidStateException extends RuntimeException {

	private static final long serialVersionUID = -1284734416019721578L;

	public InvalidStateException ( String message ) {
		super( message );
	}

}
