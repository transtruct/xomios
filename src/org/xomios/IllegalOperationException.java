/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;

/**
 * Thrown when an invalid operation is attempted.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class IllegalOperationException extends RuntimeException {

	private static final long serialVersionUID = 3334192684783170146L;

	public IllegalOperationException ( String message ) {
		super( message );
	}

}
