/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import org.xomios.internal.Socket;

/**
 * Test applications to create and close a socket
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InternalSocketTest {

	/**
	 * @param args None
	 */
	public static void main ( String[] args ) {

		System.out.println( "Test started..." );
		Socket s = new Socket();

		System.out.println( "Socket created..." );
		s.close();

		System.out.println( "Socket closed..." );
		System.out.println( "If this proceded without error or exception then the test passed" );

	}
}
