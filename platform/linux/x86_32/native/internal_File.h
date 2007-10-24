/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _INTERNAL_FILE_H
#define _INTERNAL_FILE_H

#include "xomios_platform.h"

/* TODO Determine other platforms that need some or all of these flags. */
#if defined(XOM_PLATFORM_GNU_LINUX)
# define _GNU_SOURCE
# define _LARGEFILE_SOURCE
# define _LARGEFILE64_SOURCE
# define _FILE_OFFSET_BITS 64 /* Provide 64-bit interfaces to file functions. */
#endif

#include "internal.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define XOM_INTERNAL_FILE( x ) XOM_INTERNAL( File_##x )

#define INTERNAL_FILE_O_RDONLY 1
#define INTERNAL_FILE_O_WRONLY 2
#define INTERNAL_FILE_O_RDWR 3
#define INTERNAL_FILE_O_APPEND 16
#define INTERNAL_FILE_O_CREAT 32
#define INTERNAL_FILE_O_DSYNC 64
#define INTERNAL_FILE_O_EXCL 128
#define INTERNAL_FILE_O_NOCTTY 256
#define INTERNAL_FILE_O_NONBLOCK 512
#define INTERNAL_FILE_O_RSYNC 1024
#define INTERNAL_FILE_O_SYNC 2048
#define INTERNAL_FILE_O_TRUNC 4096

/**
 * @see org.xomios.internal.File#_initialize()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1initialize ) ( JNIEnv *, jclass );

/**
 * @see org.xomios.internal.File#_open()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1open ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.File#_close()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1close ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.File#_read()
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_FILE( _1read ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.File#_setOffset()
 */
JNIEXPORT jlong JNICALL XOM_INTERNAL_FILE( _1setOffset ) ( JNIEnv *, jobject, jlong, jobject );

/**
 * (This is a fast implementation of setOffset( 0, Seek.CURRENT ) and is
 * functionally equivalent.)
 * 
 * @see org.xomios.internal.File#_getOffset()
 */
JNIEXPORT jlong JNICALL XOM_INTERNAL_FILE( _1getOffset ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.File#_write()
 */
JNIEXPORT void JNICALL XOM_INTERNAL_FILE( _1write ) ( JNIEnv *, jobject, jstring );

#endif
