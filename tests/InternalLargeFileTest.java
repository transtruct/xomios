/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 *
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import org.xomios.IOException;
import org.xomios.internal.*;

/**
 * Tests the org.xomios.internal.File class by reading and writing from large
 * files.
 *
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
class InternalLargeFileTest {
	
	public static void main ( String[] args ) throws IOException {
		System.out.println( "Beginning tests." );
		
		System.out.println( "Creating new file object (" + args[0] + ")..." );
		File f = new File( args[0] );
		
		System.out.println();
		
		System.out.println( "Opening file with O_RDONLY..." );
		f.open( File.O_RDONLY );
		System.out.println( "Seeking to end of file..." );
		System.out.println( "+" + f.setOffset( 0, File.Seek.END ) );
		System.out.println( "@" + f.getOffset() );
		f.close();
		
		System.out.println();
		
		System.out.println( "Testing concluded." );
	}
	
}