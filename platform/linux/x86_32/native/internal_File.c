/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internal_File.h"

/**
 * @see org.xomios.internal.File#open()
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
	jbyte *path = (jbyte *) (*env)->GetStringUTFChars( env, s_path, NULL );
	
	/* Open the file. */
	int fd = open( (char *) path, oflag );
	if (fd == -1) {
		/* Some error occurred, will check errno()/strerror_r() for details later. */
		jclass c_AccessException = (*env)->FindClass( env, "Lorg/xomios/connectivity/AccessException;" );
		(*env)->ThrowNew( env, c_AccessException, "file could not be opened" );
		
		return;
	}
	
	/* Locate and assign the internal file descriptor. */
	jfieldID f_File_fileDescriptor = (*env)->GetFieldID( env, c_File, "fileDescriptor", "I" );
	(*env)->SetIntField( env, this, f_File_fileDescriptor, (jint) fd );
	
	return;
}

/**
 * @see org.xomios.internal.File#close()
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
		
		return;
	}
	
	(*env)->SetIntField( env, this, f_File_fileDescriptor, (jint) -1 );
	
	return;
}

/**
 * @see org.xomios.internal.File#_get(int)
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_FILE( _1get__I ) ( JNIEnv *env, jobject this, jint length ) {
	return NULL;
}

/**
 * @see org.xomios.internal.File#_get()
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_FILE( _1get__ ) ( JNIEnv *env, jobject this ) {
	return NULL;
}
