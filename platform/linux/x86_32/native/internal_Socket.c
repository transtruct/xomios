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
#include <stdlib.h>
#include <string.h>

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
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	
	/* Socket file descriptor, address family, and socket type */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	
	/* Retrieve the values for addressFamily and socketType */
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	jint cSocket;
	int af, sock; /* Address family and socket type */
	
	af = getNativeAddressFamily( addressFamily );
	sock = getNativeSocketType ( socketType );
	
	cSocket = socket( af, sock, 0 );
	if ( cSocket < 0 ) {
		/* Error occured while creating socket */
		
		/*  
		 * TODO: Check errno for the particular error and pass this to a new
		 * SocketCreationException object
		 */
		(*env)->ThrowNew( env, SocketException_c, "Error creating socket" );
	}
	else {
		/* Store the value of the file descriptor */
		(*env)->SetIntField( env, obj, cSocket_f, cSocket );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_close ( JNIEnv *env, jobject obj ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err = close(cSocket);
	
	if ( err < 0 ) {	
		(*env)->ThrowNew( env, SocketException_c, "Error occurred while trying to close socket" );
	}
	
	/* This marks the file descriptor closed to future calls */
	cSocket = -1;
	
	(*env)->SetIntField( env, obj, cSocket_f, cSocket );
}


/*
 * Class:     org_xomios_internal_Socket
 * Method:    connect
 * Signature: ([BIII)V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_connect ( JNIEnv *env, jobject obj, jbyteArray ip, 
								jint port, jint socktype, jint addressfamily ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
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
		
		err = connect( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
		if ( err < 0 ) {
			(*env)->ThrowNew( env, SocketException_c, "Error connecting to remote host" );
		}
	}
	else if ( ipLength == 16 ) {
		/* IP version 6 */
		(*env)->ThrowNew( env, SocketException_c, "IPv6 Support not yet implemented" );
	}
	
	/* Release the memory allocated to store the IP address */
	(*env)->ReleaseByteArrayElements( env, ip, (sbyte*) ip_native, 0 );
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    bind
 * Signature: ([BIII)V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_bind ( JNIEnv *env, jobject obj, jbyteArray ip, 
								jint port, jint socktype, jint addressfamily ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
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
		
		err = bind( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
		if ( err < 0 ) {
			printf ( "%d\n", errno );
			(*env)->ThrowNew( env, SocketException_c, "Error binding to address/port" );
		}
	}
	else if ( ipLength == 16 ) {
		/* IP version 6 */
		(*env)->ThrowNew( env, SocketException_c, "IPv6 Support not yet implemented" );
	}
	
	/* Release the memory allocated to store the IP address */
	(*env)->ReleaseByteArrayElements( env, ip, (sbyte*) ip_native, 0 );
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    listen
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_listen (JNIEnv *env, jobject obj ) {
	/* Our favorite classes */
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Get the backlog value */
	jfieldID backlog_f = (*env)->GetFieldID( env, Socket_c, "backlog", "I" );
	jint backlog = (*env)->GetIntField( env, obj, backlog_f );
	
	int err;
	
	/* Perform the actual listen call */
	err = listen( cSocket, backlog );
	
	if ( err < 0 ) {
		printf( "%d\n", errno );
		(*env)->ThrowNew( env, SocketException_c, "Error setting socket to listen" );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    accept
 * Signature: ()Lorg/xomios/internal/Socket;
 */
JNIEXPORT jobject JNICALL Java_org_xomios_internal_Socket_accept (JNIEnv *env, jobject obj ) {
	/* get the classes we need */
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	jclass IPv4Address_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/IPv4Address;" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Get the address family... */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* ...and socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	/* NetworkAddress field of a Socket */
	jfieldID remoteAddress_f = (*env)->GetFieldID( env, Socket_c, "remoteAddress", "Lorg/xomios/connectivity/net/NetworkAddress;" );
	
	jmethodID constSocket_m = (*env)->GetMethodID( env, Socket_c, "<init>", "(II)V" );
	jmethodID constIPv4Address_m = (*env)->GetMethodID( env, IPv4Address_c, "<init>", "([B)V" );
	
	struct sockaddr_in remoteAddress;
	socklen_t remoteAddress_size;
	
	int newSocket, err;
	
	/* Perform the accept() call */
	err = newSocket = accept( cSocket, (struct sockaddr*) &remoteAddress, &remoteAddress_size );
	
	if ( err < 0 ) {
		(*env)->ThrowNew( env, SocketException_c, "Error accepting connection" );
	}
	
	/* Remote IP */
	sbyte ip[4];
	int ip_h = remoteAddress.sin_addr.s_addr;
	jbyteArray ip_internal = (*env)->NewByteArray( env, 4 );
	
	ip[0] = (ntohl( ip_h ) & (0xFF << 24)) >> 24;
	ip[1] = (ntohl( ip_h ) & (0xFF << 16)) >> 16;
	ip[2] = (ntohl( ip_h ) & (0xFF <<  8)) >>  8;
	ip[3] = (ntohl( ip_h ) & (0xFF <<  0)) >>  0;
	
	(*env)->SetByteArrayRegion( env, ip_internal, 0, 4, ip );
	
	/* Create a new socket object with the properties of the remote host */
	jobject clientSocket = (*env)->NewObject( env, Socket_c, constSocket_m, addressFamily, socketType );
	jobject newIPv4Address = (*env)->NewObject( env, IPv4Address_c, constIPv4Address_m, ip_internal );
	
	(*env)->SetObjectField( env, clientSocket, remoteAddress_f, newIPv4Address );
	(*env)->SetIntField( env, clientSocket, cSocket_f, newSocket );	

	return clientSocket;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recv
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_xomios_internal_Socket_recv ( JNIEnv *env, jobject obj, jint count ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err;
	char *msgbuf = malloc( (count) * sizeof(char) );
	int msglen;
	
	err = msglen = recv( cSocket, msgbuf, (count-1) * sizeof(char), 0 );

	if ( err < 0 ) {
		(*env)->ThrowNew( env, SocketException_c, "Error receiving data" );
	}

	jstring msg = (*env)->NewStringUTF( env, msgbuf );
			
	return msg;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    send
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_xomios_internal_Socket_send ( JNIEnv *env, jobject obj, jstring data ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Store return value of send call */
	int err;
	
	/* Retreive the length of the message */
	int msgLength = (*env)->GetStringLength( env, data );
	
	/* Retreive the string */
	char* msg = (char*) (*env)->GetStringUTFChars( env, data, NULL );
	
	err = send( cSocket, msg, msgLength, 0 );
	if ( err < 0 ) {
		(*env)->ThrowNew( env, SocketException_c, "Error sending message" );
	}
	
	/* Release memory allocated to store message */
	(*env)->ReleaseStringChars( env, data, (jchar*) msg );
	
	return msgLength;
}


