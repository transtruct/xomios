/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import org.xomios.internal.*;
import org.xomios.connectivity.net.*;

/**
 * Test socket server
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class ServerTest {

	/**
	 * @param args None
	 */
	public static void main ( String[] args ) throws InterruptedException {

		System.out.print( "Creating socket..." );
		  Socket s = new Socket();
		  s.createSocket();
		System.out.println( "done." );
		
		System.out.print( "Creating address object..." );
		  IPv4Address addr = new IPv4Address( "127.0.0.1", Integer.valueOf( args[0] ) );
		System.out.println( "done." );
		
		System.out.print( "Binding connection..." );
		  s.bind( addr );
		System.out.println( "done." );
		
		System.out.print( "Listening for connection..." );
		  s.listen( 5 );
		  Socket c = s.accept();
		System.out.println( "done." );
		
		System.out.println( "Host:" );
		  System.out.println( "\tRemote host: " + c.getRemoteAddress().toString());
		
		System.out.println( "Waiting for message..." );
		  System.out.println( "\t" + c.recv( 100 ) );
		System.out.println( "done." );
		
		System.out.print( "Closing client connection..." );
		  c.close();
		System.out.println( "done." );
		  
		System.out.print( "Closing server connection..." );
		  s.close();
		System.out.println( "done." );
		
		System.out.println( "\nIf this proceded without error or exception then the test passed" );

	}
}
