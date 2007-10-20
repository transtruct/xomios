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
#include <netinet/in.h>
#include <errno.h>

typedef unsigned char ubyte;
typedef signed char sbyte;

/**
 * Converts the internal constants for address family to a native constant
 */
int getNativeAddressFamily ( int addressFamily ) {
	int native_af = -1;
	switch ( addressFamily ) {
		case org_xomios_internal_Socket_AF_INET:
				native_af = AF_INET;
				break;
		case org_xomios_internal_Socket_AF_INET6:
				native_af = AF_INET6;
				break;
		case org_xomios_internal_Socket_AF_UNIX:
				native_af = AF_UNIX;
				break;
		case org_xomios_internal_Socket_AF_UNSPEC:
				native_af = AF_UNSPEC;
				break;		
	}
	return native_af;
}

/**
 * Converts the internal constants for socket address to a native constant
 */
int getNativeSocketType ( int socketType ) {
	int internal_sock = -1;
	switch ( socketType ) {
	case org_xomios_internal_Socket_SOCK_STREAM:
			internal_sock = SOCK_STREAM;
			break;
	case org_xomios_internal_Socket_SOCK_DGRAM:
			internal_sock = SOCK_DGRAM;
			break;
	case org_xomios_internal_Socket_SOCK_RAW:
			internal_sock = SOCK_RAW;
			break;
	}
	return internal_sock;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    createSocket
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_createSocket ( JNIEnv *env, jobject obj ) {
	jclass socketClass = (*env)->GetObjectClass( env, obj );
	jclass SocketException = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	
	/* Socket file descriptor, address family, and socket type */
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jfieldID addressFamily_f = (*env)->GetFieldID( env, socketClass, "addressFamily", "I" );
	jfieldID socketType_f = (*env)->GetFieldID( env, socketClass, "socketType", "I" );
	
	/* Retrieve the values for addressFamily and socketType */
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	jint socketFD;
	int af, sock; /* Address family and socket type */
	
	af = getNativeAddressFamily( addressFamily );
	sock = getNativeSocketType ( socketType );
	
	socketFD = socket( af, sock, 0 );
	if ( socketFD < 0 ) {
		/* Error occured while creating socket */
		
		/*  
		 * TODO: Check errno for the particular error and pass this to a new
		 * SocketCreationException object
		 */
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
	jclass SocketException = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jint socketFD = (*env)->GetIntField( env, obj, socketFD_f );
	
	int err = close(socketFD);
	
	if ( err < 0 ) {	
		(*env)->ThrowNew( env, SocketException, "Error occurred while trying to close socket" );
	}
	
	/* This marks the file descriptor closed to future calls */
	socketFD = (int) NULL;
	
	(*env)->SetIntField( env, obj, socketFD_f, socketFD );
}


/*
 * Class:     org_xomios_internal_Socket
 * Method:    connect
 * Signature: ([BIII)V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_connect ( JNIEnv *env, jobject obj, jbyteArray ip, 
								jint port, jint socktype, jint addressfamily ) {
	jclass socketClass = (*env)->GetObjectClass( env, obj );
	jclass SocketException = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jint socketFD = (*env)->GetIntField( env, obj, socketFD_f );
	
	/* Get the IP addr into a standard array (unsigned byte) */
	ubyte *ip_native;
	
	int ipLength = (*env)->GetArrayLength( env, ip );
	
	if ( ipLength == 4 ) {
		/* IP version 4 */
		
		/* Java bytes are all signed so we must cast */
		ip_native = (ubyte*) (*env)->GetByteArrayElements( env, ip, NULL );
		
		/* The socket address struct for the connection */
		struct sockaddr_in endpoint;
		
		/* Return value for connect() call */
		int err = 0;
		
		/* Network order address */
		in_addr_t addr = (ip_native[0] << 24) |
						 (ip_native[1] << 16) |
						 (ip_native[2] <<  8) |
						 (ip_native[3]);
		
		/* Convert the host address to network order and store */		
		endpoint.sin_addr.s_addr = htonl(addr);
		endpoint.sin_family = getNativeAddressFamily( addressfamily );
		
		if ( port > 0 ) {
			/* 
			 * Set the port if it is greater than 0. -1 represents a connection 
			 * not utilizing ports 
			 */
			endpoint.sin_port = htons((short)port);
		}
		
		err = connect( socketFD, (struct sockaddr *) &endpoint, sizeof(endpoint) );
		if ( err < 0 ) {
			(*env)->ThrowNew( env, SocketException, "Error connecting to remote host" );
		}
	}
	else if ( ipLength == 16 ) {
		/* IP version 6 */
		(*env)->ThrowNew( env, SocketException, "IPv6 Support not yet implemented" );
	}
	
	/* Release the memory allocated to store the IP address */
	(*env)->ReleaseByteArrayElements( env, ip, (sbyte*) ip_native, 0 );
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
	jclass socketClass = (*env)->GetObjectClass( env, obj );
	jclass SocketException = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID socketFD_f = (*env)->GetFieldID( env, socketClass, "cSocket", "I" );
	jint socketFD = (*env)->GetIntField( env, obj, socketFD_f );
	
	/* Store return value of send call */
	int err;
	
	/* Retreive the length of the message */
	int msgLength = (*env)->GetStringLength( env, data );
	
	/* Retreive the string */
	char* msg = (char*) (*env)->GetStringUTFChars( env, data, NULL );
	
	err = send( socketFD, msg, msgLength, 0 );
	if ( err < 0 ) {
		(*env)->ThrowNew( env, SocketException, "Error sending message" );
	}
	
	/* Release memory allocated to store message */
	(*env)->ReleaseStringChars( env, data, (jchar*) msg );
	
	return msgLength;
}


