/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

/**
 * 
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class IPv4Address extends NetAddress {

	/* IP version 4 */
	protected int ipVersion = 4;

	/**
	 * Create a new IPv4Address using the IPv4 address in byte[] address
	 * 
	 * @param address IPv4 address stored as a byte array of length 4. Uses
	 *            network order for bytes (big-endian).
	 * @throws InvalidAddressException
	 */
	public IPv4Address ( byte[] address ) throws InvalidAddressException {
		super();

		/*
		 * Store the address if it is the correct length, otherwise throw an
		 * exception
		 */
		if ( address.length == 4 ) {
			this.address = address;
		}
		else {
			throw new InvalidAddressException( "Invalid length for byte array for IP version 4 address" );
		}
	}

	@Override
	public String getHostName ( ) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String toString ( ) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected native void doAddressLookup ( );

}
