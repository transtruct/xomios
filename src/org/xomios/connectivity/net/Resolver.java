/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.connectivity.net;

/**
 * Perform DNS queries
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class Resolver {

	/**
	 * Perform an A record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of byte arrays each 4 bytes long. Each 4 byte array is
	 *         an IP version 4 address.
	 * @throws ResolutionException Thrown if the record lookup fails
	 */
	public static native NetworkAddress[] getARecord ( String host ) throws ResolutionException;

	/**
	 * Perform an AAAA record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of byte arrays each 16 bytes long. Each 16 byte array
	 *         is an IP version 6 address.
	 */
	public static native NetworkAddress[] getAAAARecord ( String host ) throws ResolutionException;

	/**
	 * Perform a NS record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of strings, each a NS record
	 * @throws ResolutionException Thrown if the record lookup fails
	 */
	public static native String[] getNSRecord ( String host ) throws ResolutionException;

	/**
	 * Perform a SOA record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return All available SOA records as an array of strings
	 */
	public static native String[] getSOARecord ( String host ) throws ResolutionException;

	/**
	 * Perform a CNAME record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of strings, each a CNAME record for the specified host
	 *         name
	 * @throws ResolutionException Thrown if the record lookup fails
	 */
	public static native String[] getCNAMERecord ( String host ) throws ResolutionException;

	/**
	 * Perform an PTR record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of strings, each a fully qualified domain name
	 * @throws ResolutionException Thrown if the record lookup fails
	 */
	public static native String[] getPTRRecord ( String host ) throws ResolutionException;

	/**
	 * Perform an MX record lookup for the specified host
	 * 
	 * @param host A fully qualified host name
	 * @return An array of MX records, each a string
	 * @throws ResolutionException Thrown if the record lookup fails
	 */
	public static native String[] getMXRecord ( String host ) throws ResolutionException;

}
