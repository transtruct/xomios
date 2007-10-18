/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internal_Socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

/*
 * Class:     org_xomios_internal_Socket
 * Method:    createSocket
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_createSocket ( JNIEnv *env, jobject obj ) {
	jclass socketClass = (*env)->GetObjectClass( env, obj );
	
	/* Socket file descriptor, address family, and socket type */
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jfieldID addressFamily_f = (*env)->GetFieldID( env, socketClass, "addressFamily", "I" );
	jfieldID socketType_f = (*env)->GetFieldID( env, socketClass, "socketType", "I" );
	
	/* Retrieve the values for addressFamily and socketType */
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	jint socketFD;
	int af, sock; /* Address family and socket type */
	
	switch ( addressFamily ) {
	case org_xomios_internal_Socket_AF_INET:
			af = AF_INET;
			break;
	case org_xomios_internal_Socket_AF_INET6:
			af = AF_INET6;
			break;
	case org_xomios_internal_Socket_AF_UNIX:
			af = AF_UNIX;
			break;
	case org_xomios_internal_Socket_AF_UNSPEC:
			af = AF_UNSPEC;
			break;			
	}
	
	switch ( socketType ) {
	case org_xomios_internal_Socket_SOCK_STREAM:
			sock = SOCK_STREAM;
			break;
	case org_xomios_internal_Socket_SOCK_DGRAM:
			sock = SOCK_DGRAM;
			break;
	case org_xomios_internal_Socket_SOCK_RAW:
			sock = SOCK_RAW;
			break;
	}
	
	socketFD = socket( af, sock, 0 );
	if ( socketFD < 0 ) {
		/* Error occured while creating socket */
		
		/*  
		 * TODO: Check errno for the particular error and pass this to a new
		 * SocketCreationException object
		 */
		jclass SocketException = (*env)->FindClass( env, "org.xomios.connectivity.net.SocketException" );
		(*env)->ThrowNew( env, SocketException, "Error creating socket" );
	}
	else {
		/* Store the value of the file descriptor */
		(*env)->SetIntField( env, obj, socketFD_f, socketFD );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_close ( JNIEnv *env, jobject obj ) {
	jclass socketClass = (*env)->GetObjectClass( env, obj );
	
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jint socketFD = (*env)->GetIntField( env, obj, socketFD_f );
	
	int err = close(socketFD);
	
	if ( err < 0 ) {
		jclass SocketException = (*env)->FindClass( env, "org.xomios.connectivity.net.SocketException" );
		(*env)->ThrowNew( env, SocketException, "Error occurred while trying to close socket" );
	}
	
	/* This marks the file descriptor closed to future calls */
	socketFD = (int) NULL;
	
	(*env)->SetIntField( env, obj, socketFD_f, socketFD );
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recv
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_xomios_internal_Socket_recv ( JNIEnv *env, jobject obj, jint count ) {
  return (jstring) "";
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    send
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_xomios_internal_Socket_send ( JNIEnv *env, jobject obj, jstring data ) {
  return 0;
}


