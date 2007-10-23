/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;

/**
 * A network address. This can be either IP version 4 or 6 and may or may not
 * be associated with a port number
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public abstract class NetworkAddress {

	protected byte[] address;

	/**
	 * Set the address for this object
	 * 
	 * @param host The host IP as a string
	 * @throws AddressFormatException the address is not properly formatted
	 */
	public abstract void setAddress ( String host ) throws AddressFormatException;

	/**
	 * Sets the address for the object
	 * 
	 * @param addr The address as an array of bytes
	 * @throws AddressFormatException The array does not represent a valid
	 *             network address
	 */
	public abstract void setAddress ( byte[] addr ) throws AddressFormatException;

	/**
	 * Get the address as an array of bytes
	 * 
	 * @return The network address as an array of bytes
	 */
	public byte[] getAddress ( ) {
		return this.address;
	}

	/**
	 * Resolve the network address to a name
	 * 
	 * @return The host name of the network address
	 * @throws ResolutionException Name resolution failed
	 */
	public String getHost ( ) throws ResolutionException {
		return Resolver.getPTRRecord( this.toString() )[0];
	}

	/**
	 * Return the version of the IP standard implemented by this object
	 * 
	 * @return The version of IP implemented by this class
	 */
	public abstract int getIPVersion ( );

	/**
	 * Return the network address formatted as a string
	 * 
	 * @return The network address in proper format as a string
	 */
	@Override
	public abstract String toString ( );

}
