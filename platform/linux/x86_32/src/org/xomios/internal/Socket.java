/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.internal;

import org.xomios.connectivity.net.AddressFormatException;
import org.xomios.connectivity.net.NetworkAddress;
import org.xomios.connectivity.net.SocketException;

/**
 * Native socket implementation
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class Socket {

	/*
	 * Network families, i.e IPv4, IPv6, DexNet, etc. This includes things such
	 * as Unix files sockets.
	 */

	/**
	 * IP version 4 socket
	 */
	public static final int AF_INET = 10;

	/**
	 * IP version 6 socket
	 */
	public static final int AF_INET6 = 11;

	/**
	 * Unix file socket
	 */
	public static final int AF_UNIX = 12;

	/**
	 * Unspecified socket type
	 */
	public static final int AF_UNSPEC = 99;


	/*
	 * The socket transmission protocol types. There correspond to things such
	 * as TCP, UDP, ICMP, etc.
	 */

	/**
	 * Reliable data delivery (TCP)
	 */
	public static final int SOCK_STREAM = 10;

	/**
	 * Unreliable data transmission (UDP)
	 */
	public static final int SOCK_DGRAM = 11;

	/**
	 * Raw socket access
	 */
	public static final int SOCK_RAW = 12;


	/* Address family (see above) and socket type (see above) */
	protected int addressFamily;
	protected int socketType;

	/**
	 * The socket file descriptor. This should never be accessed from within
	 * Java and only from C.
	 */
	protected int cSocket;


	/**
	 * Create a new socket object
	 * 
	 * @param addressFamily One of Socket.AF_*
	 * @param sockType One of Socket.SOCK_*
	 */
	public Socket ( int addressFamily, int sockType ) {
		/* Just copy these in and we let the createSocket function handle it */
		this.addressFamily = addressFamily;
		this.socketType = sockType;

		/* Create the socket file descriptor */
		this.createSocket();
	}

	/**
	 * Creates a socket object using the default socket type of SOCK_STREAM
	 * 
	 * @param addressFamily One of Socket.AF_*
	 */
	public Socket ( int addressFamily ) {

		this( addressFamily, Socket.SOCK_STREAM );

	}

	/**
	 * Create a socket object using the default address family of AF_INET and
	 * the default socket type of SOCK_STREAM
	 */
	public Socket ( ) {
		this( Socket.AF_INET, Socket.SOCK_STREAM );
	}

	/**
	 * Get a new descriptor for a low level socket connection
	 * 
	 * @return The file descriptor of the socket
	 */
	private native void createSocket ( );

	/**
	 * Destroy the internal socket connection. After this is called the socket
	 * cannot be reopened.
	 */
	public native void close ( );

	/**
	 * Connect the socket to the remove host specified by addr
	 * 
	 * @param addr A NetworkAddress
	 */
	public void connect ( NetworkAddress addr ) throws AddressFormatException, SocketException {
		if ( !addr.portSet() ) {
			throw new AddressFormatException( "Host not set in NetworkAddress object" );
		}

		this.connect( addr.getAddress(), addr.getPort(), this.socketType, this.addressFamily );
	}

	protected native void connect ( byte[] ip, int port, int socktype, int af );


	/**
	 * Read a specified number of bytes from the socket connection
	 * 
	 * @param size Number of bytes to read
	 * @return The data read as a String
	 */
	public native String recv ( int size );

	/**
	 * Send a string of bytes out of the socket
	 * 
	 * @param data A list of bytes as a String
	 * @return Number of bytes transmitted
	 */
	public native int send ( String data );

	static {
		/* Load the native library */
		System.loadLibrary( "xomios" );
	}

}
