/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internal_File.h"

/**
 * @see org.xomios.internal.File#_open()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1open ) ( JNIEnv *env, jobject this, jint options ) {
	/* Make sure the options are sane by POSIX. */
	int oflag = 0;
	
	/* Old implementations of O_RDONLY, O_WRONLY, and O_RDWR let O_RDONLY == 0,
	 * so these need to be checked independently.
	 */
	if ((options & INTERNAL_FILE_O_RDWR) == INTERNAL_FILE_O_RDWR) {
		oflag |= O_RDWR;
	}
	else if ((options & INTERNAL_FILE_O_WRONLY) == INTERNAL_FILE_O_WRONLY) {
		oflag |= O_WRONLY;
	}
	else {
		/* Assume O_RDONLY. */
		oflag |= O_RDONLY;
	}
	
	/* Need to account for O_NONBLOCK and open()s on invalid streams... */
	
	oflag |= ( options & INTERNAL_FILE_O_APPEND ) ? O_APPEND : 0;
	oflag |= ( options & INTERNAL_FILE_O_CREAT ) ? O_CREAT : 0;
	oflag |= ( options & INTERNAL_FILE_O_DSYNC ) ? O_DSYNC : 0;
	oflag |= ( options & INTERNAL_FILE_O_EXCL ) ? O_EXCL : 0;
	oflag |= ( options & INTERNAL_FILE_O_NOCTTY ) ? O_NOCTTY : 0;
	oflag |= ( options & INTERNAL_FILE_O_NONBLOCK ) ? O_NONBLOCK : 0;
	oflag |= ( options & INTERNAL_FILE_O_RSYNC ) ? O_RSYNC : 0;
	oflag |= ( options & INTERNAL_FILE_O_SYNC ) ? O_SYNC : 0;
	oflag |= ( options & INTERNAL_FILE_O_TRUNC ) ? O_TRUNC : 0;
	
	/* Locate classes. */
	jclass c_File = (*env)->GetObjectClass( env, this );
	
	/* Locate methods. */
	jmethodID m_File_getPath = (*env)->GetMethodID( env, c_File, "getPath", "()Ljava/lang/String;" );
	
	jstring s_path = (jstring) (*env)->CallObjectMethod( env, this, m_File_getPath );
	const jbyte *path = (const jbyte *) (*env)->GetStringUTFChars( env, s_path, NULL );
	
	/* Open the file. */
	int fd = open( (char *) path, oflag );
	if (fd == -1) {
		/* Some error occurred, will check errno()/strerror_r() for details later. */
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ReleaseStringUTFChars( env, s_path, (const char *)path );
		(*env)->ThrowNew( env, c_AccessException, "file could not be opened" );
	}
	
	(*env)->ReleaseStringUTFChars( env, s_path, (const char *)path );
	
	/* Locate and assign the internal file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	(*env)->SetIntField( env, this, f_File_fileDescriptor, (jint) fd );
	
	return;
}

/**
 * @see org.xomios.internal.File#_read()
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_FILE( _1read ) ( JNIEnv *env, jobject this, jint length ) {
	char buffer[length + 1];
	jclass c_File = (*env)->GetObjectClass( env, this );
	
	/* Get the file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	int fd = (int) (*env)->GetIntField( env, this, f_File_fileDescriptor );
	
	/* Wipe the buffer. */
	memset( buffer, '\0', length + 1 );
	
	if (read( fd, buffer, length ) == -1) {
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ThrowNew( env, c_AccessException, "file could not be read" );
	}
	
	return (*env)->NewStringUTF( env, buffer );
}

/**
 * @see org.xomios.internal.File#_write()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1write ) ( JNIEnv *env, jobject this, jstring data ) {
	const jbyte *buffer = (const jbyte *) (*env)->GetStringUTFChars( env, data, NULL );
	jclass c_File = (*env)->GetObjectClass( env, this );
	
	/* Get the file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	int fd = (int) (*env)->GetIntField( env, this, f_File_fileDescriptor );
	
	if (write(fd, (void *)buffer, (*env)->GetStringUTFLength( env, data )) == -1) {
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ReleaseStringUTFChars( env, data, (const char *)buffer );
		(*env)->ThrowNew( env, c_AccessException, "file could not be written" );
	}
	
	(*env)->ReleaseStringUTFChars( env, data, (const char *)buffer );
	
	return;
}

/**
 * @see org.xomios.internal.File#_seek()
 */
JNIEXPORT jint JNICALL XOM_INTERNAL_FILE( _1seek ) ( JNIEnv *env, jobject this, jint offset, jobject o_whence ) {
	jclass c_File = (*env)->GetObjectClass( env, this );
	jclass c_File_Seek = (*env)->GetObjectClass( env, o_whence );
	
	/* How are we going to seek? */
	jfieldID f_File_Seek_CURRENT = (*env)->GetStaticFieldID( env, c_File_Seek, "CURRENT", "Lorg/xomios/internal/File$Seek;" );
	jfieldID f_File_Seek_END = (*env)->GetStaticFieldID( env, c_File_Seek, "END", "Lorg/xomios/internal/File$Seek;" );
	
	int whence;
	off_t new_offset;
	
	if ((*env)->IsSameObject( env, o_whence, (*env)->GetStaticObjectField( env, c_File_Seek, f_File_Seek_CURRENT ) ) == JNI_TRUE) {
		whence = SEEK_CUR;
	}
	else if ((*env)->IsSameObject( env, o_whence, (*env)->GetStaticObjectField( env, c_File_Seek, f_File_Seek_END ) ) == JNI_TRUE) {
		whence = SEEK_END;
	}
	else /* o_whence == Seek.SET or something went disastrously wrong. */ {
		whence = SEEK_SET;
	}
	
	/* Get the file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	int fd = (int) (*env)->GetIntField( env, this, f_File_fileDescriptor );
	
	if ((new_offset = lseek(fd, (off_t)offset, whence)) == (off_t)-1) {
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ThrowNew( env, c_AccessException, "could not seek in file" );
	}
	
	return offset;
}

/**
 * @see org.xomios.internal.File#_close()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1close ) ( JNIEnv *env, jobject this ) {	
	jclass c_File = (*env)->GetObjectClass( env, this );
	
	/* Get the file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	int fd = (int) (*env)->GetIntField( env, this, f_File_fileDescriptor );
	
	/* Close the file. */
	if (close(fd) == -1) {
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ThrowNew( env, c_AccessException, "file could not be closed" );
	}
	
	(*env)->SetIntField( env, this, f_File_fileDescriptor, (jint) -1 );
	
	return;
}
