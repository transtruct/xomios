/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;

/**
 * Thrown when a resource limit is reached.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ResourceLimitException extends ResourceException {

	private static final long serialVersionUID = -5789397471195220527L;

	public ResourceLimitException ( String message ) {
		super( message );
	}

}
