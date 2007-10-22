/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.internal;

import org.xomios.connectivity.InvalidStateException;
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

	/* The size of the connection queue */
	protected int backlog = 0;

	/**
	 * The socket file descriptor. This should never be accessed from within
	 * Java and only from C.
	 */
	protected int cSocket;

	/**
	 * This points to a NetworkAddress object describing the remote host after
	 * a connect or in a Socket object returned by accept()
	 */
	protected NetworkAddress remoteAddress = null;

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

	protected Socket ( boolean dummy ) {

	}

	/**
	 * Create the new descriptor for a low level socket connection
	 */
	public native void createSocket ( );

	/**
	 * Destroy the internal socket connection. After this is called the socket
	 * cannot be reopened.
	 */
	public native void close ( );

	/**
	 * Connect the socket to the remove host specified by addr
	 * 
	 * @param addr A NetworkAddress
	 * @throws AddressFormatException the specified address is incomplete. i.e.
	 *             You have specified a SOCK_STREAM socket but the port is not
	 *             set as part of the NetworkAddress
	 * @throws SocketException An internal error has occurred while connecting
	 */
	public void connect ( NetworkAddress addr ) throws AddressFormatException,
			SocketException {
		if ( ( this.socketType == Socket.SOCK_STREAM || this.socketType == Socket.SOCK_DGRAM )
				&& !addr.portSet() ) {
			throw new AddressFormatException( "Host not set in NetworkAddress object but is required by specified socket type" );
		}

		if ( addr.portSet() ) {
			this.connect( addr.getAddress(), addr.getPort(), this.socketType, this.addressFamily );
		}
		else {
			this.connect( addr.getAddress(), -1, this.socketType, this.addressFamily );
		}
	}

	/**
	 * Performs the actual connection operation at the native level
	 */
	protected native void connect ( byte[] ip, int port, int socktype, int af );

	/**
	 * Bind the socket to the specified host and port
	 * 
	 * @param addr A NetworkAddress
	 * @throws AddressFormatException the specified address is incomplete. i.e.
	 *             You have specified a SOCK_STREAM socket but the port is not
	 *             set as part of the NetworkAddress
	 * @throws SocketException An internal error has occurred while connecting
	 */
	public void bind ( NetworkAddress addr ) throws AddressFormatException,
			SocketException {
		if ( ( this.socketType == Socket.SOCK_STREAM || this.socketType == Socket.SOCK_DGRAM )
				&& !addr.portSet() ) {
			throw new AddressFormatException( "Host not set in NetworkAddress object but is required by specified socket type" );
		}

		if ( addr.portSet() ) {
			this.bind( addr.getAddress(), addr.getPort(), this.socketType, this.addressFamily );
		}
		else {
			this.bind( addr.getAddress(), -1, this.socketType, this.addressFamily );
		}
	}

	/**
	 * Perform the actual bind operation at the native level
	 */
	protected native void bind ( byte[] ip, int port, int socktype, int af );

	/**
	 * Set the socket as actively listening for incoming connections
	 * 
	 * @param backlog Size of the queue of connections waiting
	 * @throws SocketException an internal error has occurred
	 */
	public void listen ( int backlog ) throws SocketException {
		this.backlog = backlog;
		this.listen();
	}

	/**
	 * Perform the listen operation at a low level
	 */
	protected native void listen ( ) throws SocketException;

	/**
	 * Return the first connection from the connection queue.
	 * 
	 * @return A new socket object used to communicate with the new remote
	 *         client
	 * @throws SocketException An error has occurred while accepting the
	 *             connection
	 */
	public native Socket accept ( ) throws SocketException;

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

	/**
	 * Returns the NetworkAddress associated with the remote end of this socket
	 * object
	 * 
	 * @return The NetworkAddress corresponding to the remote end of this
	 *         connection
	 * @throws InvalidStateException the socket is not in a state that
	 *             corresponds with a remote address. i.e. this socket is
	 *             locally bound and is listening for incoming connections
	 */
	public NetworkAddress getRemoteAddress ( ) throws InvalidStateException {
		if ( this.remoteAddress != null ) {
			return this.remoteAddress;
		}
		else {
			throw new InvalidStateException( "This socket does not have a logical remote address" );
		}
	}

	static {
		/* Load the native library */
		System.loadLibrary( "xomios" );
	}

}
