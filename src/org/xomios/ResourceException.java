/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios;

/**
 * Thrown when the state of a resource becomes inconsistent or an unexpected
 * change occurs in a resource.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ResourceException extends RuntimeException {

	private static final long serialVersionUID = 7337444913694143670L;

	public ResourceException ( String message ) {
		super( message );
	}

}
