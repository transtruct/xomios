/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import org.xomios.internal.*;
import org.xomios.connectivity.net.*;
import org.xomios.connectivity.*;
import org.xomios.*;

/**
 * Test socket server
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class Socket_ServerUDP {

	/**
	 * @param args None
	 */
	public static void main ( String[] args ) throws InterruptedException, IOException {

		UDPEndPoint c = new UDPEndPoint( new IPv4Address("0.0.0.0"), 0 );
		String msg;
		
		System.out.print( "Creating socket..." );
		  Socket s = new Socket( Socket.AF_INET, Socket.SOCK_DGRAM );
		  s.createSocket();
		System.out.println( "done." );
		
		System.out.print( "Creating address object..." );
		  IPv4Address addr = new IPv4Address( "127.0.0.1" );
		  UDPEndPoint lhost = new UDPEndPoint( addr, Integer.valueOf( args[0] ) );
		System.out.println( "done." );
		
		System.out.print( "Binding connection..." );
		  s.bind( lhost );
		System.out.println( "done." );
		
		System.out.print( "Listening for connection..." );
		  msg = s.recvfrom( 100, c );
		System.out.println( "done." );
		
		System.out.println( "Host:" );
		  System.out.println( "\tRemote host: " + c.getNetworkAddress().toString() + ":" + c.getPort() );
		
		System.out.println( "Waiting for message..." );
		  System.out.println( "\t" + msg );
		System.out.println( "done." );
		
		System.out.print( "Sending message back to client..." );
		  s.sendto( "Hai2u2", c );
		System.out.println( "done." );
		
		System.out.print( "Closing server connection..." );
		  s.close();
		System.out.println( "done." );
		
		System.out.println( "\nIf this proceded without error or exception then the test passed" );

	}
}
