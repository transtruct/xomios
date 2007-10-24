/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internal_File.h"

#include <errno.h>

static jfieldID f_File_fileDescriptor;

#define INTERNAL_FILE_FD_SET( env, object, value ) (*(env))->SetIntField( (env), (object), f_File_fileDescriptor, (jint) (value) )
#define INTERNAL_FILE_FD_GET( env, object ) ((int) (*(env))->GetIntField( (env), (object), f_File_fileDescriptor ))
#define INTERNAL_FILE_FD_CLEAR( env, object ) INTERNAL_FILE_FD_SET( (env), (object), -1 )

/**
 * @see org.xomios.internal.File#_initialize()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1initialize ) ( JNIEnv *env, jclass class ) {
	f_File_fileDescriptor = (*env)->GetFieldID( env, class, "fileDescriptor", "I" );
}

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
	
	/* Locate methods. */
	jclass c_File = (*env)->GetObjectClass( env, this );
	jmethodID m_File_getPath = (*env)->GetMethodID( env, c_File, "getPath", "()Ljava/lang/String;" );
	
	jstring s_path = (jstring) (*env)->CallObjectMethod( env, this, m_File_getPath );
	const jbyte *path = (const jbyte *) (*env)->GetStringUTFChars( env, s_path, NULL );
	
	/* Open the file. */
	int fd;
	if ((fd = open( (char *) path, oflag )) < 0) {
		/* Clean up path. */
		(*env)->ReleaseStringUTFChars( env, s_path, (const char *) path );
		xomios_error_throw( env, errno );
		
		return;
	}
	
	(*env)->ReleaseStringUTFChars( env, s_path, (const char *) path );
	
	INTERNAL_FILE_FD_SET( env, this, fd );
	
	return;
}

/**
 * @see org.xomios.internal.File#_read()
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_FILE( _1read ) ( JNIEnv *env, jobject this, jint length ) {
	char buffer[length + 1];
	
	/* Get the file descriptor. */
	int fd = INTERNAL_FILE_FD_GET( env, this );
	
	/* Wipe the buffer. */
	memset( buffer, '\0', length + 1 );
	
	if (read( fd, buffer, length ) == -1) {
		xomios_error_throw( env, errno );
		
		return (jstring) NULL;
	}
	
	return (*env)->NewStringUTF( env, buffer );
}

/**
 * @see org.xomios.internal.File#_write()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1write ) ( JNIEnv *env, jobject this, jstring data ) {
	const jbyte *buffer = (const jbyte *) (*env)->GetStringUTFChars( env, data, NULL );
	
	/* Get the file descriptor. */
	int fd = INTERNAL_FILE_FD_GET( env, this );
	
	if (write( fd, (void *) buffer, (*env)->GetStringUTFLength( env, data ) ) == -1) {
		(*env)->ReleaseStringUTFChars( env, data, (const char *) buffer );
		xomios_error_throw( env, errno );
		
		return;
	}
	
	(*env)->ReleaseStringUTFChars( env, data, (const char *)buffer );
	
	return;
}

/**
 * @see org.xomios.internal.File#_setOffset()
 */
JNIEXPORT jlong JNICALL XOM_INTERNAL_FILE( _1setOffset ) ( JNIEnv *env, jobject this, jlong offset, jobject o_whence ) {
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
	int fd = INTERNAL_FILE_FD_GET( env, this );
	
	if ((new_offset = lseek(fd, (off_t) offset, whence)) == (off_t) -1) {
		xomios_error_throw( env, errno );
		
		return (jlong) 0;
	}
	
	return (jlong) new_offset;
}

/**
 * (This is a fast implementation of setOffset( 0, Seek.CURRENT ) and is
 * functionally equivalent.)
 * 
 * @see org.xomios.internal.File#_getOffset()
 */
JNIEXPORT jlong JNICALL XOM_INTERNAL_FILE( _1getOffset ) ( JNIEnv *env, jobject this ) {
	int fd = INTERNAL_FILE_FD_GET( env, this );
	
	off_t offset;
	
	if ((offset = lseek(fd, (off_t) 0, SEEK_CUR)) == (off_t) -1) {
		xomios_error_throw( env, errno );
		
		return (jlong) 0;
	}
	
	return (jlong) offset;
}

/**
 * @see org.xomios.internal.File#_close()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1close ) ( JNIEnv *env, jobject this ) {	
	int fd = INTERNAL_FILE_FD_GET( env, this );
	
	/* Close the file. */
	if (close( fd ) == -1) {
		xomios_error_throw( env, errno );
		
		return;
	}
	
	INTERNAL_FILE_FD_CLEAR( env, this );
	
	return;
}
