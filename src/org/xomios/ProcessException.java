/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;

/**
 * Thrown when a process performs unexpectedly in comparison to its normal
 * behavior.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ProcessException extends RuntimeException {

	private static final long serialVersionUID = 3343593404431973662L;

	public ProcessException ( String message ) {
		super( message );
	}

}
