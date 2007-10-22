/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _INTERNAL_SOCKET_H
#define _INTERNAL_SOCKET_H

#include "internal.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define R_INTERNAL_SOCKET( x ) R_INTERNAL( Socket_##x )

#define INTERNAL_SOCKET_AF_INET 10
#define INTERNAL_SOCKET_AF_INET6 11
#define INTERNAL_SOCKET_AF_UNIX 12
#define INTERNAL_SOCKET_AF_UNSPEC 99
#define INTERNAL_SOCKET_SOCK_STREAM 10
#define INTERNAL_SOCKET_SOCK_DGRAM 11
#define INTERNAL_SOCKET_SOCK_RAW 12

/**
 * @see org.xomios.internal.Socket#createSocket()
 */
JNIEXPORT void JNICALL R_INTERNAL_SOCKET( createSocket ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#close()
 */
JNIEXPORT void JNICALL R_INTERNAL_SOCKET( close ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#connect()
 */
JNIEXPORT void JNICALL R_INTERNAL_SOCKET( connect ) ( JNIEnv *, jobject, jbyteArray, jint, jint, jint );

/**
 * @see org.xomios.internal.Socket#bind()
 */
JNIEXPORT void JNICALL R_INTERNAL_SOCKET( bind ) ( JNIEnv *, jobject, jbyteArray, jint, jint, jint );

/**
 * @see org.xomios.internal.Socket#listen()
 */
JNIEXPORT void JNICALL R_INTERNAL_SOCKET( listen ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#accept()
 */
JNIEXPORT jobject JNICALL R_INTERNAL_SOCKET( accept ) ( JNIEnv *, jobject );

/**
 * @see org.xomios.internal.Socket#recv()
 */
JNIEXPORT jstring JNICALL R_INTERNAL_SOCKET( recv ) (JNIEnv *, jobject, jint);

/**
 * @see org.xomios.internal.Socket#send()
 */
JNIEXPORT jint JNICALL R_INTERNAL_SOCKET( send ) ( JNIEnv *, jobject, jstring );

#endif
