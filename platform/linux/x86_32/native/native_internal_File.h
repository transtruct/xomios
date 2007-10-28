/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _NATIVE_INTERNAL_FILE_H
#define _NATIVE_INTERNAL_FILE_H

#include <xomios_platform.h>

/* TODO Determine other platforms that need some or all of these flags. */
#if defined(XOM_PLATFORM_GNU_LINUX)
# define _GNU_SOURCE
# define _LARGEFILE_SOURCE
# define _LARGEFILE64_SOURCE
# define _FILE_OFFSET_BITS 64 /* Provide 64-bit interfaces to file functions. */
#endif

#include "native_internal.h"
#include <xomios_error.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define XOM_NATIVE_INTERNAL_FILE( x ) XOM_NATIVE_INTERNAL( File_##x )

#define XOM_NATIVE_INTERNAL_FILE_O_RDONLY 1
#define XOM_NATIVE_INTERNAL_FILE_O_WRONLY 2
#define XOM_NATIVE_INTERNAL_FILE_O_RDWR 3
#define XOM_NATIVE_INTERNAL_FILE_O_APPEND 16
#define XOM_NATIVE_INTERNAL_FILE_O_CREAT 32
#define XOM_NATIVE_INTERNAL_FILE_O_DSYNC 64
#define XOM_NATIVE_INTERNAL_FILE_O_EXCL 128
#define XOM_NATIVE_INTERNAL_FILE_O_NOCTTY 256
#define XOM_NATIVE_INTERNAL_FILE_O_NONBLOCK 512
#define XOM_NATIVE_INTERNAL_FILE_O_RSYNC 1024
#define XOM_NATIVE_INTERNAL_FILE_O_SYNC 2048
#define XOM_NATIVE_INTERNAL_FILE_O_TRUNC 4096

/**
 * @see org.xomios.internal.File#_initialize()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_FILE( _1initialize ) ( JNIEnv *, jclass );

/**
 * @see org.xomios.internal.File#_open()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_FILE( _1open ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.File#_close()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_FILE( _1close ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.File#_read()
 */
JNIEXPORT jstring JNICALL XOM_NATIVE_INTERNAL_FILE( _1read ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.File#_setOffset()
 */
JNIEXPORT jlong JNICALL XOM_NATIVE_INTERNAL_FILE( _1setOffset ) ( JNIEnv *, jobject, jlong, jobject );

/**
 * (This is a fast implementation of setOffset( 0, Seek.CURRENT ) and is
 * functionally equivalent.)
 * 
 * @see org.xomios.internal.File#_getOffset()
 */
JNIEXPORT jlong JNICALL XOM_NATIVE_INTERNAL_FILE( _1getOffset ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.File#_write()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_FILE( _1write ) ( JNIEnv *, jobject, jstring );

#endif
