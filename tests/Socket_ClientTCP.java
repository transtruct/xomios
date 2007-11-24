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
 * Test socket client
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class Socket_ClientTCP {

	/**
	 * @param args None
	 */
	public static void main ( String[] args ) throws SocketException, IOException {

		System.out.print( "Creating socket..." );
		  Socket s = new Socket( Socket.AF_INET, Socket.SOCK_STREAM );
		  s.createSocket();
		System.out.println( "done." );
		
		System.out.print( "Creating address object..." );
		  IPv4Address addr = new IPv4Address( "127.0.0.1" );
		  TCPEndPoint rhost = new TCPEndPoint( addr, Integer.valueOf( args[0] ) );
		System.out.println( "done." );
		
		System.out.print( "Binding connection..." );
		  s.connect( rhost );
		System.out.println( "done." );
		
		System.out.print( "Sending message..." );
		  s.send( "Hello world!" );
		System.out.println( "done." );
		
		System.out.println( "Waiting for reply..." );
		  System.out.println( "\t" + s.recv( 100 ) );
		System.out.println( "done." );
		  
		System.out.print( "Closing connection..." );
		  s.close();
		System.out.println( "done." );
		
		System.out.println( "\nIf this proceded without error or exception then the test passed" );

	}
}
