/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 *
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

import java.util.EnumSet;

import org.xomios.IOException;
import org.xomios.internal.*;

/**
 * Tests the org.xomios.internal.File class.
 *
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class InternalFileTest {

	public static void main ( String[] args ) throws IOException {
		System.out.println( "Beginning tests." );
		
		System.out.println( "Creating new file object (" + args[0] + ")..." );
		File f = new File( args[0] );
		
		System.out.println( "Opening file with O_RDONLY..." );
		f.open( File.O_RDONLY );
		System.out.println( "Reading 32 bytes from file..." );
		String s = f.read(32);
		System.out.println(s);
		System.out.println( "Reading remainder of file..." );
		s = f.read();
		System.out.println(s);
		System.out.println( "Closing file..." );
		f.close();
		
		System.out.println();
		
		System.out.println( "Opening file with O_WRONLY | O_APPEND... ");
		f.open( File.O_WRONLY | File.O_APPEND );
		System.out.println( "Writing data to file." );
		f.write( "So, I herd u liek mudkipz.\n" );
		System.out.println( "Closing file..." );
		f.close();
		
		System.out.println();
		
		System.out.println( "Opening file with O_RDONLY..." );
		f.open( File.O_RDONLY );
		System.out.println( "Seeking to byte 32 in file..." );
		f.seek( 32 );
		System.out.println( "Seeking an additional 4 bytes... " );
		f.seek( 4, File.Seek.CURRENT );
		System.out.println( "Reading remainder of file..." );
		s = f.read();
		System.out.println(s);
		System.out.println( "Seeking 4 bytes from the end..." );
		f.seek( -4, File.Seek.END );
		System.out.println( "Reading remainder of file..." );
		s = f.read();
		System.out.println(s);
		System.out.println( "Closing file..." );
		f.close();
		
		System.out.println();
		
		System.out.println( "Testing concluded." );
	}

}
