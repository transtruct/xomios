/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.location;

/**
 * Represents the location of a system-accessible file.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class FileLocation extends Location {

	private final String path;

	public FileLocation ( String path ) {
		this.path = path;
	}

	public String getPath ( ) {
		return this.path;
	}

	@Override
	public String toString ( ) {
		return this.getPath();
	}

}
