/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * The port at the end of an TCP or UDP connection for example
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public interface NetworkPort {

	/**
	 * Set the port number for this class
	 * 
	 * @param p The port number
	 */
	public void setPort ( int p );

	/**
	 * Get the port number
	 * 
	 * @return The port number
	 */
	public int getPort ( );

	/**
	 * Get the maximum port number supported
	 * 
	 * @return The maximum port number supported
	 */
	public int getMaxPortNumber ( );

	/**
	 * Determines if the port attribute is set on this class
	 * 
	 * @return True if the port is set, false otherwise
	 */
	public boolean portSet ( );

}
