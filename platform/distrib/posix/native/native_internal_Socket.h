/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _INTERNAL_SOCKET_H
#define _INTERNAL_SOCKET_H

#include "native_internal.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define XOM_NATIVE_INTERNAL_SOCKET( x ) XOM_NATIVE_INTERNAL( Socket_##x )

#define XOM_NATIVE_INTERNAL_SOCKET_AF_INET 10
#define XOM_NATIVE_INTERNAL_SOCKET_AF_INET6 11
#define XOM_NATIVE_INTERNAL_SOCKET_AF_UNIX 12
#define XOM_NATIVE_INTERNAL_SOCKET_AF_UNSPEC 99
#define XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM 10
#define XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM 11
#define XOM_NATIVE_INTERNAL_SOCKET_SOCK_RAW 12

/**
 * @see org.xomios.internal.Socket#createSocket()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1createSocket ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#close()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1close ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#connect()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1connect ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.Socket#bind()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1bind ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.Socket#listen()
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1listen ) ( JNIEnv *, jobject, jint );

/**
 * @see org.xomios.internal.Socket#accept()
 */
JNIEXPORT jobject JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1accept ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#recv()
 */
JNIEXPORT jstring JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1recv ) (JNIEnv *, jobject, jint);

/**
 * @see org.xomios.internal.Socket#recvfrom()
 */
JNIEXPORT jstring JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1recvfrom ) (JNIEnv *, jobject, jint, jobject);

/**
 * @see org.xomios.internal.Socket#send()
 */
JNIEXPORT jint JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1send ) ( JNIEnv *, jobject, jstring );

/**
 * @see org.xomios.internal.Socket#sendto()
 */
JNIEXPORT jint JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1sendto ) ( JNIEnv *, jobject, jstring, jobject );

#endif
