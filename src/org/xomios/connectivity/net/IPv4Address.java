/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;


/**
 * A IP version 4 address with an option port number associated with it
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class IPv4Address extends NetworkAddress {

	/**
	 * TODO This belongs somewhere else -- ports are not associated with IP
	 * addresses.
	 * 
	 * Fair enough but I couldn't come up with a better way to have socket
	 * handle it. Let me know if you think of anything because I really don't
	 * like this either.
	 * 
	 * Fixed. Moved to ConnectionEndPoint.
	 */

	/**
	 * Create a new address object bound to the specified address
	 * 
	 * @param addr The IP to associate this object with the host
	 * @throws AddressFormatException address is not a properly formatted IPv4
	 *             address
	 */
	public IPv4Address ( String address ) throws AddressFormatException {
		this.setAddress( address );
	}

	/**
	 * Create a new address object bound to the specified address
	 * 
	 * @param addr The IP to associate this object with the host
	 * @throws AddressFormatException address does not represent a valid IPv4
	 *             address
	 */
	public IPv4Address ( byte[] address ) throws AddressFormatException {
		this.setAddress( address );
	}

	/**
	 * @see org.xomios.connectivity.net.NetworkAddress#setAddress(java.lang.String)
	 */
	@Override
	public void setAddress ( String address ) throws AddressFormatException {
		String[] octets = address.split( "\\." );
		byte[] addr = new byte[4];

		if ( octets.length != 4 ) {
			throw new AddressFormatException( address + " is not a valid address" );
		}

		for ( int i = 0; i < 4; i++ ) {
			try {
				addr[i] = (byte) ( Integer.valueOf( octets[i] ) & ( (byte)0xff ) );
			}
			catch ( NumberFormatException e ) {
				throw new AddressFormatException( address + " is not a valid address" );
			}
		}

		this.address = addr;
	}

	/**
	 * @see org.xomios.connectivity.net.NetworkAddress#setAddress(byte[])
	 */
	@Override
	public void setAddress ( byte[] addr ) throws AddressFormatException {
		if ( addr.length != 4 ) {
			throw new AddressFormatException( "byte[] of length " + String.valueOf( addr.length ) + " is not valid" );
		}

		this.address = addr;
	}

	/**
	 * @see org.xomios.connectivity.net.NetworkAddress#getIPVersion()
	 */
	@Override
	public int getIPVersion ( ) {
		return 4;
	}

	/**
	 * @see org.xomios.connectivity.net.NetworkAddress#toString()
	 */
	@Override
	public String toString ( ) {
		StringBuffer buf = new StringBuffer();

		for ( int i = 0; i < 4; i++ ) {
			buf.append( this.address[i] & 0xff );
			if ( i != 3 ) {
				buf.append( "." );
			}
		}

		return buf.toString();
	}

}
