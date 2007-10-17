/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;

/**
 * Abstract representation of a network IP address; IPv4 or IPv6.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class NetworkAddress {

	/*
	 * Host as a sequence of bytes. The length of this depends on the IP
	 * version being used
	 */
	protected byte[] address;

	/*
	 * Host name of the specified system. If this a value hasn't already been
	 * determined though user input it is determined by reverse DNS look up as
	 * supported by the operating system. Default is null (not set).
	 */
	protected String host = null;

	/**
	 * Version of IP used. At the time this can be 4 or 6.
	 */
	protected int ipVersion;

	/**
	 * Private constructor. Retrieve instances using getIPv<x>Address
	 * functions.
	 */
	private NetworkAddress ( ) {
		/* Do nothing... */
	}

	/**
	 * Retrieve an address object based on the target host name
	 * 
	 * @param host The host name of the system
	 * @return A new NetworkAddress object
	 * @throws ResolutionException Thrown when address lookup for the specified
	 *             host name fails
	 */
	public static NetworkAddress getIPv4Address ( String host ) throws ResolutionException {
		NetworkAddress naddr = new NetworkAddress();
		naddr.address = Resolver.getARecord( host )[0];

		return naddr;
	}

	/**
	 * Retrieve a new NetworkAddress object
	 * 
	 * @param addr The IP version 4 address of the host
	 * @return A new NetworkAddress object
	 */
	public static NetworkAddress getIPv4Address ( byte[] addr ) {
		NetworkAddress naddr = new NetworkAddress();
		naddr.address = addr;

		return naddr;
	}

	/**
	 * Get the IP version supported by this class
	 * 
	 * @return The IP version this class supports
	 */
	public int getIPVersion ( ) {
		return this.ipVersion;
	}

	/**
	 * Get the specified IP address of the host
	 * 
	 * @return A byte array representing the bytes of the address. The length
	 *         of this depends on the IP address type.
	 */
	public byte[] getAddress ( ) {
		return this.address;
	}

	/**
	 * Get the host name. If it is not immediately accessible a reverse DNS
	 * lookup is performed to determine the host name
	 * 
	 * @return The host name of the system or the IP as a string if a host name
	 *         can not be found
	 */
	public String getHostName ( ) throws ResolutionException {
		if ( this.host == null ) {
			this.host = Resolver.getPTRRecord( this.address )[0];
		}

		return this.host;
	}

	/**
	 * Return either the host name or the IP address as a string
	 * 
	 * @return The host name as a string if it is already cached internally. If
	 *         the host name is not immediately accessible the IP address is
	 *         returned as a string
	 */
	@Override
	public String toString ( ) {
		return null;
	}

}
