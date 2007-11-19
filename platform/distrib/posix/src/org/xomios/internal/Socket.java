/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.internal;

import org.xomios.connectivity.net.AddressFormatException;
import org.xomios.connectivity.net.ConnectionEndPoint;
import org.xomios.connectivity.net.NetworkPort;
import org.xomios.connectivity.net.SocketException;
import org.xomios.connectivity.net.TCPEndPoint;
import org.xomios.connectivity.net.UDPEndPoint;


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
	 * The socket file descriptor. Passed to C functions.
	 */
	protected int cSocket = -1;

	boolean connected = false;
	boolean binded = false;
	boolean listening = false;

	/**
	 * This points to a ConnectionEndPoint object describing the attached host
	 * after a connect/bind or in a Socket object returned by accept()
	 */
	protected ConnectionEndPoint attachedHost = null;

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
	 * Create a new file descriptor for the socket
	 */
	public void createSocket ( ) throws SocketException {
		this._createSocket();
	}

	protected native void _createSocket ( ) throws SocketException;

	/**
	 * Destroy the internal socket connection. After this is called the socket
	 * cannot be reopened.
	 */
	public void close ( ) throws SocketException, IllegalStateException {
		if ( this.cSocket < 0 ) {
			throw new IllegalStateException( "socket not created. can not close" );
		}
		this._close();
	}

	protected native void _close ( ) throws SocketException;

	/**
	 * Connect the socket to the remote host specified by host
	 * 
	 * @param addr A NetworkAddress
	 * @throws AddressFormatException the specified address is incomplete. i.e.
	 *             You have specified a SOCK_STREAM socket but the port is not
	 *             set as part of the NetworkAddress
	 * @throws SocketException An internal error has occurred while connecting
	 */
	public void connect ( ConnectionEndPoint host ) throws IllegalArgumentException, SocketException {
		if ( this.socketType == Socket.SOCK_STREAM && ! ( host instanceof TCPEndPoint ) ) {
			throw new IllegalArgumentException( "Socket is of type SOCK_STREAM but ConnectionEndPoint does not not describe a TCP enabled connection" );
		}
		else if ( this.socketType == Socket.SOCK_DGRAM && ! ( host instanceof UDPEndPoint ) ) {
			throw new IllegalArgumentException( "Socket is of type SOCK_DGRAM but ConnectionEndPoint does not not describe a UDP enabled connection" );
		}
		else {
			this.attachedHost = host;
			this._connect( ( (NetworkPort)host ).getPort() );
			this.connected = true;
		}
	}

	/**
	 * Performs the actual connection operation at the native level
	 */
	protected native void _connect ( int port );

	/**
	 * Bind the socket to the specified host and port
	 * 
	 * @param addr A NetworkAddress
	 * @throws AddressFormatException the specified address is incomplete. i.e.
	 *             You have specified a SOCK_STREAM socket but the port is not
	 *             set as part of the NetworkAddress
	 * @throws SocketException An internal error has occurred while connecting
	 */
	public void bind ( ConnectionEndPoint host ) throws IllegalArgumentException, SocketException {
		if ( this.socketType == Socket.SOCK_STREAM && ! ( host instanceof TCPEndPoint ) ) {
			throw new IllegalArgumentException( "Socket is of type SOCK_STREAM but ConnectionEndPoint does not not describe a TCP enabled connection" );
		}
		else if ( this.socketType == Socket.SOCK_DGRAM && ! ( host instanceof UDPEndPoint ) ) {
			throw new IllegalArgumentException( "Socket is of type SOCK_DGRAM but ConnectionEndPoint does not not describe a UDP enabled connection" );
		}
		else {
			this.attachedHost = host;
			this._bind( ( (NetworkPort)host ).getPort() );
			this.binded = true;
		}
	}

	/**
	 * Perform the actual bind operation at the native level
	 */
	protected native void _bind ( int port );

	/**
	 * Set the socket as actively listening for incoming connections
	 * 
	 * @param backlog Size of the queue of connections waiting
	 * @throws SocketException an internal error has occurred
	 */
	public void listen ( int backlog ) throws SocketException, IllegalStateException {
		if ( !this.binded ) {
			throw new IllegalStateException( "socket not bound to host" );
		}
		this._listen( backlog );
		this.listening = true;
	}

	/**
	 * Perform the listen operation at a low level
	 */
	protected native void _listen ( int backlog ) throws SocketException;

	/**
	 * Return the first connection from the connection queue.
	 * 
	 * @return A new socket object used to communicate with the new remote
	 *         client
	 * @throws SocketException An error has occurred while accepting the
	 *             connection
	 */
	public Socket accept ( ) throws SocketException, IllegalStateException {
		if ( this.cSocket < 0 ) {
			throw new IllegalStateException( "socket not created" );
		}
		else if ( !this.binded ) {
			throw new IllegalStateException( "socket not bound to address" );
		}
		else if ( !this.listening ) {
			throw new IllegalStateException( "socket not listening for connections" );
		}
		Socket sClient = this._accept();
		sClient.connected = true;

		return sClient;
	}

	protected native Socket _accept ( ) throws SocketException;

	/**
	 * Read a specified number of bytes from the socket connection
	 * 
	 * @param size Number of bytes to read
	 * @return The data read as a String
	 */
	public String recv ( int size ) {
		if ( this.cSocket < 0 ) {
			throw new IllegalStateException( "socket not created" );
		}
		else if ( !this.connected ) {
			throw new IllegalStateException( "socket not connected" );
		}

		return this._recv( size );

	}

	protected native String _recv ( int size );

	/**
	 * Send a string of bytes out of the socket
	 * 
	 * @param data A list of bytes as a String
	 * @return Number of bytes transmitted
	 */
	public int send ( String data ) {
		if ( this.cSocket < 0 ) {
			throw new IllegalStateException( "socket not created" );
		}
		else if ( !this.connected ) {
			throw new IllegalStateException( "socket not connected" );
		}

		return this._send( data );

	}

	protected native int _send ( String data );

	/**
	 * Returns the NetworkAddress associated with the remote end of this socket
	 * object
	 * 
	 * @return The NetworkAddress corresponding to the remote end of this
	 *         connection
	 * @throws IllegalStateException the socket is not in a state that
	 *             corresponds with a remote address. i.e. this socket is
	 *             locally bound and is listening for incoming connections
	 */
	public ConnectionEndPoint getAttachedHost ( ) throws IllegalStateException {
		if ( this.attachedHost != null ) {
			return this.attachedHost;
		}
		else {
			throw new IllegalStateException( "not connected to a logical remote host" );
		}
	}

	static {
		/* Load the native library */
		System.loadLibrary( "xomios" );
	}

}
