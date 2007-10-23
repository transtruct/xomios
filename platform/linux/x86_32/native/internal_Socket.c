/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internal_Socket.h"

typedef unsigned char ubyte;
typedef signed char sbyte;

/**
 * Converts the internal constants for address family to a native constant
 */
int getNativeAddressFamily ( int addressFamily ) {
	int native_af = -1;
	switch ( addressFamily ) {
		case INTERNAL_SOCKET_AF_INET:
				native_af = AF_INET;
				break;
		case INTERNAL_SOCKET_AF_INET6:
				native_af = AF_INET6;
				break;
		case INTERNAL_SOCKET_AF_UNIX:
				native_af = AF_UNIX;
				break;
		case INTERNAL_SOCKET_AF_UNSPEC:
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
	case INTERNAL_SOCKET_SOCK_STREAM:
			internal_sock = SOCK_STREAM;
			break;
	case INTERNAL_SOCKET_SOCK_DGRAM:
			internal_sock = SOCK_DGRAM;
			break;
	case INTERNAL_SOCKET_SOCK_RAW:
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
JNIEXPORT void JNICALL XOM_INTERNAL_SOCKET( createSocket ) ( JNIEnv *env, jobject obj ) {
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
JNIEXPORT void JNICALL XOM_INTERNAL_SOCKET( close ) ( JNIEnv *env, jobject obj ) {
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
JNIEXPORT void JNICALL XOM_INTERNAL_SOCKET( connect ) ( JNIEnv *env, jobject obj, jint port ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	jclass NetworkAddress_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkAddress;" );
	jclass ConnectionEndPoint_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* And the address family */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* And the socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	/* And the connection end point */
	jfieldID attachedHost_f = (*env)->GetFieldID( env, Socket_c, "attachedHost", "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	jobject attachedHost = (*env)->GetObjectField( env, obj, attachedHost_f );
	
	/* And lets grab the NetworkAddress */
	jmethodID getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;");
	jobject networkAddress = (*env)->CallObjectMethod( env, attachedHost, getNetworkAddress_m );	
			
	/* Get the IP address into a standard array (unsigned byte) */
	jmethodID getAddress_m = (*env)->GetMethodID( env, NetworkAddress_c, "getAddress", "()[B" );
	jbyteArray ip = (*env)->CallObjectMethod( env, networkAddress, getAddress_m );
	
	/* Get the IP version */
	jmethodID getIPVersion_m = (*env)->GetMethodID( env, NetworkAddress_c, "getIPVersion", "()I" );
	jint ipVersion = (*env)->CallIntMethod( env, networkAddress, getIPVersion_m );
	
	/* The native byte array IP */
	ubyte *ip_native;

	/* Return value for connect() call */
	int err;
	
	if ( ipVersion == 4 ) {
		/* IP version 4 */
		int native_socketType = getNativeAddressFamily( socketType );
		
		if ( native_socketType == SOCK_STREAM || native_socketType == SOCK_DGRAM ) {		
			/* Java bytes are all signed so we must cast */
			ip_native = (ubyte*) (*env)->GetByteArrayElements( env, ip, NULL );
									
			/* The socket address struct for the connection */
			struct sockaddr_in endpoint;
			
			/* Network order address */
			in_addr_t addr = (ip_native[0] << 24) |
							 (ip_native[1] << 16) |
							 (ip_native[2] <<  8) |
							 (ip_native[3]);
			
			/* Convert the host address to network order and store */		
			endpoint.sin_addr.s_addr = htonl(addr);
			endpoint.sin_family = getNativeAddressFamily( addressFamily );
			endpoint.sin_port = htons((short)port);
			
			err = connect( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
			if ( err < 0 ) {
				(*env)->ThrowNew( env, SocketException_c, "Error connecting to remote host" );
			}
		}
	}
	else if ( ipVersion == 6 ) {
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
JNIEXPORT void JNICALL XOM_INTERNAL_SOCKET( bind ) ( JNIEnv *env, jobject obj, jint port ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	jclass NetworkAddress_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkAddress;" );
	jclass ConnectionEndPoint_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* And the address family */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* And the socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	/* And the connection end point */
	jfieldID attachedHost_f = (*env)->GetFieldID( env, Socket_c, "attachedHost", "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	jobject attachedHost = (*env)->GetObjectField( env, obj, attachedHost_f );
	
	/* And lets grab the NetworkAddress */
	jmethodID getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;");
	jobject networkAddress = (*env)->CallObjectMethod( env, attachedHost, getNetworkAddress_m );	
			
	/* Get the IP address */
	jmethodID getAddress_m = (*env)->GetMethodID( env, NetworkAddress_c, "getAddress", "()[B" );
	jbyteArray ip = (*env)->CallObjectMethod( env, networkAddress, getAddress_m );
	
	/* Get the IP version */
	jmethodID getIPVersion_m = (*env)->GetMethodID( env, NetworkAddress_c, "getIPVersion", "()I" );
	jint ipVersion = (*env)->CallIntMethod( env, networkAddress, getIPVersion_m );
	
	/* The native byte array IP */
	ubyte *ip_native;

	/* Return value for connect() call */
	int err;
	
	if ( ipVersion == 4 ) {
		/* IP version 4 */
		int native_socketType = getNativeAddressFamily( socketType );
		
		if ( native_socketType == SOCK_STREAM || native_socketType == SOCK_DGRAM ) {		
			/* Java bytes are all signed so we must cast */
			ip_native = (ubyte*) (*env)->GetByteArrayElements( env, ip, NULL );
			
			/* The socket address struct for the connection */
			struct sockaddr_in endpoint;
			
			/* Network order address */
			in_addr_t addr = (ip_native[0] << 24) |
							 (ip_native[1] << 16) |
							 (ip_native[2] <<  8) |
							 (ip_native[3]);
			
			/* Convert the host address to network order and store */		
			endpoint.sin_addr.s_addr = htonl(addr);
			endpoint.sin_family = getNativeAddressFamily( addressFamily );
			endpoint.sin_port = htons((short)port);
			
			err = bind( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
			if ( err < 0 ) {
				(*env)->ThrowNew( env, SocketException_c, "Error binding to host" );
			}
		}
	}
	else if ( ipVersion == 6 ) {
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
JNIEXPORT void JNICALL XOM_INTERNAL_SOCKET( listen ) (JNIEnv *env, jobject obj ) {
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
JNIEXPORT jobject JNICALL XOM_INTERNAL_SOCKET( accept ) (JNIEnv *env, jobject obj ) {
	/* get the classes we need */
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
	jclass ConnectionEndPoint_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	jclass NetworkAddress_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkAddress;" );
		
	/* Socket constructor */
	jmethodID constSocket_m = (*env)->GetMethodID( env, Socket_c, "<init>", "(II)V" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Get the address family... */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* ...and socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	
	/**
	 * The next three blocks are just to determine the version of IP in effect. 
	 * Isn't JNI fun?!?  
	 */
	
	/* And the connection end point */
	jfieldID attachedHost_f = (*env)->GetFieldID( env, Socket_c, "attachedHost", "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	jobject attachedHost = (*env)->GetObjectField( env, obj, attachedHost_f );
	
	/* And lets grab the NetworkAddress */
	jmethodID getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;");
	jobject networkAddress = (*env)->CallObjectMethod( env, attachedHost, getNetworkAddress_m );	
	
	/* Get the IP version */
	jmethodID getIPVersion_m = (*env)->GetMethodID( env, NetworkAddress_c, "getIPVersion", "()I" );
	jint ipVersion = (*env)->CallIntMethod( env, networkAddress, getIPVersion_m );
	
	
	/* The Socket object returned by this call */
	jobject clientSocket;
	
	if ( ipVersion == 4 ) {
		
		/* Lets get us a class */
		jclass IPv4Address_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/IPv4Address;" );
		jmethodID constIPv4Address_m = (*env)->GetMethodID( env, IPv4Address_c, "<init>", "([B)V" );
		
		/* 
		 * The transport class (probably TCP or UDP) and the method ID for the 
		 * constructor of this class 
		 */
		jclass transport_c;
		jmethodID constTransport_m;
		
		struct sockaddr_in remoteAddress;
		socklen_t remoteAddress_size = sizeof(remoteAddress);
		
		int newSocket, err;
		
		/* Perform the accept() call */
		err = newSocket = accept( cSocket, (struct sockaddr*) &remoteAddress, &remoteAddress_size );
		
		if ( err < 0 ) {
			(*env)->ThrowNew( env, SocketException_c, "Error accepting connection" );
		}
		
		/* Remote IP */
		sbyte ip[4];
		int ip_native = remoteAddress.sin_addr.s_addr;
		
		/* 
		 * The java internal representation of the IP as an array of bytes 
		 * (byte[]) 
		 */
		jbyteArray ip_internal = (*env)->NewByteArray( env, 4 );

		/* Break up the integer into an array of bytes representing the IP */
		ip[0] = (ntohl( ip_native ) & (0xFF << 24)) >> 24;
		ip[1] = (ntohl( ip_native ) & (0xFF << 16)) >> 16;
		ip[2] = (ntohl( ip_native ) & (0xFF <<  8)) >>  8;
		ip[3] = (ntohl( ip_native ) & (0xFF <<  0)) >>  0;
		
		/* Copy the contents to the java array */
		(*env)->SetByteArrayRegion( env, ip_internal, 0, 4, ip );
		
		/* Remote port */
		int remote_port = remoteAddress.sin_port;
		
		/* 
		 * Instantiate transport_c and contTransport_m as defined by the socket
		 * type
		 */
		if ( getNativeSocketType(socketType) == SOCK_STREAM ) {
			transport_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/TCPEndPoint;" );
			constTransport_m = (*env)->GetMethodID( env, transport_c, "<init>", "(Lorg/xomios/connectivity/net/NetworkAddress;I)V" );
		}
		else if (  getNativeSocketType(socketType) == SOCK_DGRAM ) {
			(*env)->ThrowNew( env, SocketException_c, "Transport method not implemented for accept();" );
		}
		else {
			(*env)->ThrowNew( env, SocketException_c, "Transport can not use accept()" );
		}
		
		/* Create a new socket object with the properties of the remote host */
		clientSocket = (*env)->NewObject( env, Socket_c, constSocket_m, addressFamily, socketType );
		jobject newNetworkAddress = (*env)->NewObject( env, IPv4Address_c, constIPv4Address_m, ip_internal );
		
		/* Create the connection endpoint object */
		jobject newTransportEndPoint = (*env)->NewObject( env, transport_c, constTransport_m, newNetworkAddress, remote_port );
		
		/* Move over the fields */
		(*env)->SetObjectField( env, clientSocket, attachedHost_f, newTransportEndPoint );
		(*env)->SetIntField( env, clientSocket, cSocket_f, newSocket );	
	}
	
	return clientSocket;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recv
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL XOM_INTERNAL_SOCKET( recv ) ( JNIEnv *env, jobject obj, jint count ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err;
	char *msgbuf = malloc( (count) * sizeof(char) );
	int msglen;
	
	/* Zero our memory, eh? Good idea! */
	memset( msgbuf, '\0', count );
	
	err = msglen = recv( cSocket, msgbuf, (count-1) * sizeof(char), 0 );

	if ( err < 0 ) {
		(*env)->ThrowNew( env, SocketException_c, "Error receiving data" );
	}

	jstring msg = (*env)->NewStringUTF( env, msgbuf );
	
	free( msgbuf );
	
	return msg;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    send
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL XOM_INTERNAL_SOCKET( send ) ( JNIEnv *env, jobject obj, jstring data ) {
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


