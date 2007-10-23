/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * An end point in a TCP connection
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class TCPEndPoint extends ConnectionEndPoint implements NetworkPort {

	protected final int MAX_PORT = 65535;
	protected int port;

	public TCPEndPoint ( NetworkAddress address, int port ) {
		this.setNetworkAddress( address );
		this.setPort( port );
	}

	public int getMaxPortNumber ( ) {
		return this.MAX_PORT;
	}

	public void setPort ( int port ) {
		this.port = port;
	}

	public int getPort ( ) {
		return this.port;
	}

}
