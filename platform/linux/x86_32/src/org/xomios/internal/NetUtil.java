/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.internal;

import org.xomios.connectivity.NameLookupException;

/**
 * Various networking related helper functions used elsewhere in the code base
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class NetUtil {

	/**
	 * Performs a DNS lookup on the specified host to determine its IP address
	 * 
	 * @param host A string representing a host name
	 * @return An IP address corresponding with the specified host. The length
	 *         depends on the IP address version (4 or 6).
	 * @throws NameLookupException An error occurred during name resolution.
	 *             Usually this means that the name does not resolve to a IP
	 *             address but, can also imply and error occurred.
	 */
	public static native byte[] DNSLookup ( String host ) throws NameLookupException;

}
