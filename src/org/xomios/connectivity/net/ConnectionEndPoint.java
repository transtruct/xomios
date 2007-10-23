/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * This class represents the end point of a network connection
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public abstract class ConnectionEndPoint {

	protected NetworkAddress address;

	/**
	 * Get the NetworkAddress associated with this connection end point
	 * 
	 * @return the NetworkAddress associated with this connection end point
	 */
	public NetworkAddress getNetworkAddress ( ) {
		return this.address;
	}

	/**
	 * Set the NetworkAddress associated with this connection end point
	 * 
	 * @param address the NetworkAddress associated with this connection end
	 *            point
	 */
	public void setNetworkAddress ( NetworkAddress address ) {
		this.address = address;
	}

}
