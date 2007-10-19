/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Represents the lowest application-level input/output paradigm. A
 * <code>Transport</code> simply provides methods for reading and writing
 * data.
 * 
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 */
public abstract class Transport {

	private final CopyOnWriteArrayList<TransportListener> transportListeners = new CopyOnWriteArrayList<TransportListener>();

	/**
	 * Adds a new transport listener to this transport.
	 * 
	 * @param tl A transport listener.
	 */
	public void addTransportListener ( TransportListener tl ) {
		this.transportListeners.add( tl );
	}

	/**
	 * Removes a transport listener from this transport.
	 * 
	 * @param tl A transport listener.
	 */
	public void removeTransportListener ( TransportListener tl ) {
		if ( !this.transportListeners.contains( tl ) ) {
			return;
		}

		this.transportListeners.remove( tl );
	}

}
