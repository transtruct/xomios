/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

package org.xomios.internal;

import org.xomios.IOException;
import org.xomios.connectivity.InvalidStateException;

/**
 * Native POSIX file implementation.
 * 
 * @author Christopher Thunes <cthunes@xomios.brewtab.com>
 * @author Noah Fontes <nfontes@xomios.brewtab.com>
 */
public class File {

	static {
		System.loadLibrary( "xomios" );
	}

	/* These are stolen from the Open Group's descriptions. */

	/**
	 * Open for reading only.
	 */
	public static final int O_RDONLY = 0x0001;

	/**
	 * Open for writing only.
	 */
	public static final int O_WRONLY = 0x0002;

	/**
	 * Open for reading and writing.
	 */
	public static final int O_RDWR = O_RDONLY | O_WRONLY;

	/**
	 * If set, the file offset shall be set to the end of the file prior to
	 * each write.
	 */
	public static final int O_APPEND = 0x0010;

	/**
	 * If the file exists, this flag has no effect except as noted under O_EXCL
	 * below. Otherwise, the file shall be created; the user ID of the file
	 * shall be set to the effective user ID of the process; the group ID of
	 * the file shall be set to the group ID of the file's parent directory or
	 * to the effective group ID of the process; and the access permission bits
	 * (see <sys/stat.h>) of the file mode shall be set to the value of the
	 * third argument taken as type mode_t modified as follows: a bitwise AND
	 * is performed on the file-mode bits and the corresponding bits in the
	 * complement of the process' file mode creation mask. Thus, all bits in
	 * the file mode whose corresponding bit in the file mode creation mask is
	 * set are cleared. When bits other than the file permission bits are set,
	 * the effect is unspecified. The third argument does not affect whether
	 * the file is open for reading, writing, or for both. Implementations
	 * shall provide a way to initialize the file's group ID to the group ID of
	 * the parent directory. Implementations may, but need not, provide an
	 * implementation-defined way to initialize the file's group ID to the
	 * effective group ID of the calling process.
	 */
	public static final int O_CREAT = 0x0020;

	/**
	 * Write I/O operations on the file descriptor shall complete as defined by
	 * synchronized I/O data integrity completion.
	 */
	public static final int O_DSYNC = 0x0040;

	/**
	 * If O_CREAT and O_EXCL are set, open() shall fail if the file exists. The
	 * check for the existence of the file and the creation of the file if it
	 * does not exist shall be atomic with respect to other threads executing
	 * open() naming the same filename in the same directory with O_EXCL and
	 * O_CREAT set. If O_EXCL and O_CREAT are set, and path names a symbolic
	 * link, open() shall fail and set errno to [EEXIST], regardless of the
	 * contents of the symbolic link. If O_EXCL is set and O_CREAT is not set,
	 * the result is undefined.
	 */
	public static final int O_EXCL = 0x0080;

	/**
	 * If set and path identifies a terminal device, open() shall not cause the
	 * terminal device to become the controlling terminal for the process.
	 */
	public static final int O_NOCTTY = 0x0100;

	public static final int O_NONBLOCK = 0x0200;

	/**
	 * Read I/O operations on the file descriptor shall complete at the same
	 * level of integrity as specified by the O_DSYNC and O_SYNC flags. If both
	 * O_DSYNC and O_RSYNC are set in oflag, all I/O operations on the file
	 * descriptor shall complete as defined by synchronized I/O data integrity
	 * completion. If both O_SYNC and O_RSYNC are set in flags, all I/O
	 * operations on the file descriptor shall complete as defined by
	 * synchronized I/O file integrity completion.
	 */
	public static final int O_RSYNC = 0x0400;

	/**
	 * Write I/O operations on the file descriptor shall complete as defined by
	 * synchronized I/O file integrity completion.
	 */
	public static final int O_SYNC = 0x0800;

	/**
	 * If the file exists and is a regular file, and the file is successfully
	 * opened O_RDWR or O_WRONLY, its length shall be truncated to 0, and the
	 * mode and owner shall be unchanged. It shall have no effect on FIFO
	 * special files or terminal device files. Its effect on other file types
	 * is implementation-defined. The result of using O_TRUNC with O_RDONLY is
	 * undefined.
	 */
	public static final int O_TRUNC = 0x1000;

	/**
	 * The path to the file to access.
	 * 
	 * TODO Implement this as a FileLocation.
	 */
	private final String path;

	/**
	 * Whether the file is currently opened.
	 */
	private int fileDescriptor = -1;

	/**
	 * Constructs and opens a new file.
	 * 
	 * @param path The path to the file to open.
	 */
	public File ( String path ) {
		this.path = path;
	}

	/**
	 * Gets the current file path.
	 * 
	 * @return The path to the file in use.
	 */
	public String getPath ( ) {
		return this.path;
	}

	/**
	 * Determines whether the file is currently open.
	 * 
	 * @return True if the file is open; false otherwise.
	 */
	public boolean isOpen ( ) {
		return this.fileDescriptor != -1;
	}

	/**
	 * Opens a file descriptor to the given file with the specified options.
	 * 
	 * @param options A bit mask of options to use to open the file.
	 */
	public void open ( int options ) throws IOException {
		if ( this.isOpen() ) {
			throw new InvalidStateException( "opened" );
		}
		this._open( options );
	}

	private native void _open ( int options );

	/**
	 * Reads a specified number of bytes from the given file.
	 * 
	 * @param length The number of bytes to read.
	 * 
	 * @return The contents of the file.
	 */
	public String get ( int length ) throws IOException {
		if ( !this.isOpen() ) {
			throw new InvalidStateException( "closed" );
		}
		return this._get( length );
	}

	public native String _get ( int length );

	/**
	 * Reads the entire file into memory.
	 * 
	 * @return The contents of the file.
	 */
	public String get ( ) throws IOException {
		if ( !this.isOpen() ) {
			throw new InvalidStateException( "closed" );
		}
		return this._get();
	}

	public native String _get ( );

	/**
	 * Closes an open file descriptor to the file.
	 */
	public void close ( ) throws IOException {
		if ( !this.isOpen() ) {
			throw new InvalidStateException( "closed" );
		}
		this._close();
	}

	public native void _close ( );

}
