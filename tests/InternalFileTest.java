/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 *
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import java.util.EnumSet;

import org.xomios.internal.*;

/**
 * LULZ!
 *
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InternalFileTest {

	/**
	 * @param args None
	 */
	public static void main ( String[] args ) {
		System.out.println( "Beginning tests." );
		System.out.println( "Creating new File..." );
		File f = new File( "/etc/passwd" );
		System.out.println( "Opening file with O_RDONLY..." );
		f.open( File.O_RDONLY );
		System.out.println( "Closing file..." );
		f.close();
		System.out.println( "Testing concluded." );
	}

}
