/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

import java.util.EventObject;

/**
 * Event object for transport events.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class TransportEvent extends EventObject {

	private static final long serialVersionUID = 6661854034898869348L;

	public TransportEvent ( Transport source ) {

		super( source );

	}

}
