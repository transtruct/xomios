/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

/**
 * Default implementation of TransportListener interface.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class TransportAdapter implements TransportListener {

	@Override
	public void transportClosed ( TransportEvent te ) {
	}

	@Override
	public void transportOpened ( TransportEvent te ) {
	}

	@Override
	public void transportRead ( TransportEvent te ) {
	}

	@Override
	public void transportWritten ( TransportEvent te ) {
	}

}
