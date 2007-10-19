/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

import java.util.EventListener;

/**
 * Listener for transport events.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public interface TransportListener extends EventListener {

	/**
	 * Called when a transport is opened.
	 * 
	 * @param te A transport event.
	 */
	public void transportOpened ( TransportEvent te );

	/**
	 * Called when a transport is closed.
	 * 
	 * @param te A transport event.
	 */
	public void transportClosed ( TransportEvent te );

	/**
	 * Called when a transport is written.
	 * 
	 * @param te A transport event.
	 */
	public void transportWritten ( TransportEvent te );

	/**
	 * Called when a transport is read.
	 * 
	 * @param te A transport event.
	 */
	public void transportRead ( TransportEvent te );

}
