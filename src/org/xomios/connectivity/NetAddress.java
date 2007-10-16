/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity;

/**
 * Abstract representation of a network IP address; IPv4 or IPv6.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public abstract class NetAddress {

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
	 * Retrieve an address object based on the target host name
	 * 
	 * @param host The host name of the system
	 * @throws AddressLookupException Thrown when address lookup for the
	 *             specified host name fails
	 */
	public NetAddress ( String host ) throws AddressLookupException {
		this.host = host;
		this.doAddressLookup();
	}

	/**
	 * Default constructor called by extending classes to implement their own
	 * constructors
	 */
	public NetAddress ( ) {

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
	 *         of this depends on the IP version.
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
	public abstract String getHostName ( );

	/**
	 * Return either the host name or the IP address as a string
	 * 
	 * @return The host name as a string if it is already cached internally. If
	 *         the host name is not immediately accessible the IP address is
	 *         returned as a string
	 */
	@Override
	public abstract String toString ( );

	/**
	 * Using the internally stored value for host performs and address lookup,
	 * and if successful stores the IP address in address
	 * 
	 * @throws AddressLookupException Thrown if the address lookup failed to
	 *             return a IP address
	 */
	protected abstract void doAddressLookup ( ) throws AddressLookupException;

}
