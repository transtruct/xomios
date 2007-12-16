/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "native_internal_Socket.h"
#include "xomios_error.h"

typedef unsigned char ubyte;
typedef signed char sbyte;

/**
 * Converts the internal constants for address family to a native constant
 */
static inline int getNativeAddressFamily ( int addressFamily ) {
	switch ( addressFamily ) {
		case XOM_NATIVE_INTERNAL_SOCKET_AF_INET:
				return AF_INET;
		case XOM_NATIVE_INTERNAL_SOCKET_AF_INET6:
				return AF_INET6;
		case XOM_NATIVE_INTERNAL_SOCKET_AF_UNIX:
				return AF_UNIX;
		case XOM_NATIVE_INTERNAL_SOCKET_AF_UNSPEC:
				return AF_UNSPEC;		
	}
	return -1;
}

/**
 * Converts the internal constants for socket address to a native constant
 */
static inline int getNativeSocketType ( int socketType ) {
	switch ( socketType ) {
	case XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM:
			return SOCK_STREAM;
	case XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM:
			return SOCK_DGRAM;
	case XOM_NATIVE_INTERNAL_SOCKET_SOCK_RAW:
			return SOCK_RAW;
	}
	return -1;
}

/**
 * Throw a standardized exception not implemented features
 */
static void throw_notImplemented( JNIEnv *env ) {
	jclass Exception_c = (*env)->FindClass( env, "Ljava/lang/Exception;" );
	(*env)->ThrowNew( env, Exception_c, "Feature not implemented" );
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    createSocket
 * Signature: ()V
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1createSocket ) ( JNIEnv *env, jobject obj ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	
	/* Socket file descriptor, address family, and socket type */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	
	/* Retrieve the values for addressFamily and socketType */
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	jint cSocket;
	int native_addressFamily, native_socketType; /* Address family and socket type */
	
	native_addressFamily = getNativeAddressFamily( addressFamily );
	native_socketType = getNativeSocketType ( socketType );
	
	cSocket = socket( native_addressFamily, native_socketType, 0 );
	if ( cSocket < 0 ) {
		/* Error occured while creating socket */
		xomios_error_throw( env, errno );
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
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1close ) ( JNIEnv *env, jobject obj ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err = close(cSocket);
	
	if ( err < 0 ) {
		xomios_error_throw( env, errno );
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
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1connect ) ( JNIEnv *env, jobject obj, jint port ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
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
	jmethodID ConnectionEndPoint_getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;");
	jobject networkAddress = (*env)->CallObjectMethod( env, attachedHost, ConnectionEndPoint_getNetworkAddress_m );	
			
	/* Get the IP address into a standard array (unsigned byte) */
	jmethodID NetworkAddress_getAddress_m = (*env)->GetMethodID( env, NetworkAddress_c, "getAddress", "()[B" );
	jbyteArray ip = (*env)->CallObjectMethod( env, networkAddress, NetworkAddress_getAddress_m );
	
	/* The native byte array IP */
	ubyte *native_ip;

	/* Return value for connect() call */
	int err;
	
	if ( addressFamily == XOM_NATIVE_INTERNAL_SOCKET_AF_INET ) {
		if ( socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM || socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM ) {		
			/* Java bytes are all signed so we must cast */
			native_ip = (ubyte*) (*env)->GetByteArrayElements( env, ip, NULL );
									
			/* The socket address struct for the connection */
			struct sockaddr_in endpoint;
			
			/* Convert the host address to network order and store */
			endpoint.sin_addr.s_addr = htonl( (native_ip[0] << 24) |
							 				  (native_ip[1] << 16) |
							 				  (native_ip[2] <<  8) |
							 				  (native_ip[3]) );
			endpoint.sin_family = getNativeAddressFamily( addressFamily );
			endpoint.sin_port = htons((short)port);
			
			err = connect( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
			if ( err < 0 ) {
				xomios_error_throw( env, errno );
			}
			
			/* Release memory */
			(*env)->ReleaseByteArrayElements( env, ip, (sbyte*) native_ip, 0 );
		}
		else {
			throw_notImplemented( env );
		}
	}
	else {
		/* IP version 6 */
		throw_notImplemented( env );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    bind
 * Signature: ([BIII)V
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1bind ) ( JNIEnv *env, jobject obj, jint port ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
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
	jmethodID ConnectionEndPoint_getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;");
	jobject networkAddress = (*env)->CallObjectMethod( env, attachedHost, ConnectionEndPoint_getNetworkAddress_m );	
			
	/* Get the IP address into a standard array (unsigned byte) */
	jmethodID NetworkAddress_getAddress_m = (*env)->GetMethodID( env, NetworkAddress_c, "getAddress", "()[B" );
	jbyteArray ip = (*env)->CallObjectMethod( env, networkAddress, NetworkAddress_getAddress_m );
	
	/* The native byte array IP */
	ubyte *native_ip;

	/* Return value for connect() call */
	int err;
	
	if ( addressFamily == XOM_NATIVE_INTERNAL_SOCKET_AF_INET ) {
		if ( socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM || socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM ) {		
			/* Java bytes are all signed so we must cast */
			native_ip = (ubyte*) (*env)->GetByteArrayElements( env, ip, NULL );
			
			/* The socket address struct for the connection */
			struct sockaddr_in endpoint;
			
			/* Convert the host address to network order and store */
			endpoint.sin_addr.s_addr = htonl( (native_ip[0] << 24) |
							 				  (native_ip[1] << 16) |
							 				  (native_ip[2] <<  8) |
							 				  (native_ip[3]) );
			endpoint.sin_family = getNativeAddressFamily( addressFamily );
			endpoint.sin_port = htons((short)port);
			
			err = bind( cSocket, (struct sockaddr *) &endpoint, sizeof(endpoint) );
			if ( err < 0 ) {
				xomios_error_throw( env, errno );
			}
			
			/* Release memory */
			(*env)->ReleaseByteArrayElements( env, ip, (sbyte*) native_ip, 0 );
		}
		else {
			throw_notImplemented( env );
		}
	}
	else {
		/* IP version 6 */
		throw_notImplemented( env );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    listen
 * Signature: ()V
 */
JNIEXPORT void JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1listen ) (JNIEnv *env, jobject obj, jint backlog ) {
	/* Our favorite classes */
	jclass Socket_c = (*env)->GetObjectClass( env, obj );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err;
	
	/* Perform the actual listen call */
	err = listen( cSocket, backlog );
	
	if ( err < 0 ) {
		xomios_error_throw( env, errno );
	}
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    accept
 * Signature: ()Lorg/xomios/internal/Socket;
 */
JNIEXPORT jobject JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1accept ) (JNIEnv *env, jobject obj ) {
	/* get the classes we need */
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass SocketException_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/SocketException;" );
		
	/* Socket constructor */
	jmethodID Socket_m = (*env)->GetMethodID( env, Socket_c, "<init>", "(II)V" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Get the address family... */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* ...and socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	/* The Socket object returned by this call */
	jobject clientSocket;
	
	jfieldID attachedHost_f = (*env)->GetFieldID( env, Socket_c, "attachedHost", "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
	
	if ( addressFamily == XOM_NATIVE_INTERNAL_SOCKET_AF_INET ) {
		/* Lets get us a class */
		jclass IPv4Address_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/IPv4Address;" );
		jmethodID IPv4Address_m = (*env)->GetMethodID( env, IPv4Address_c, "<init>", "([B)V" );
		
		/* 
		 * The transport class (probably TCP or UDP) and the method ID for the 
		 * constructor of this class 
		 */
		jclass Transport_c;
		jmethodID Transport_m;
		
		struct sockaddr_in native_remoteAddress;
		socklen_t native_remoteAddress_size = sizeof(native_remoteAddress);
		
		int native_clientSocketFD, err;
		
		/* Perform the accept() call */
		err = native_clientSocketFD = accept( cSocket, (struct sockaddr*) &native_remoteAddress, &native_remoteAddress_size );
		
		if ( err < 0 ) {
			xomios_error_throw( env, errno );
		}
		
		/* Remote IP */
		sbyte native_ip[4];
		int native_ip_raw = ntohl( native_remoteAddress.sin_addr.s_addr );
		
		/* 
		 * The java internal representation of the IP as an array of bytes 
		 * (byte[]) 
		 */
		jbyteArray ip = (*env)->NewByteArray( env, 4 );

		/* Break up the integer into an array of bytes representing the IP */
		native_ip[0] = ( native_ip_raw >> 24 ) & 0xFF;
		native_ip[1] = ( native_ip_raw >> 16 ) & 0xFF;
		native_ip[2] = ( native_ip_raw >>  8 ) & 0xFF;
		native_ip[3] = native_ip_raw & 0xFF;
		
		/* Copy the contents to the java array */
		(*env)->SetByteArrayRegion( env, ip, 0, 4, native_ip );
		
		/* Remote port */
		int native_port = ntohs( native_remoteAddress.sin_port );
		
		/* 
		 * Instantiate transport_c and contTransport_m as defined by the socket
		 * type
		 */
		if ( socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM ) {
			Transport_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/TCPEndPoint;" );
			Transport_m = (*env)->GetMethodID( env, Transport_c, "<init>", "(Lorg/xomios/connectivity/net/NetworkAddress;I)V" );
		}
		else {
			(*env)->ThrowNew( env, SocketException_c, "Transport can not utilize accept()" );
		}
		
		/* Create a new socket object with the properties of the remote host */
		clientSocket = (*env)->NewObject( env, Socket_c, Socket_m, addressFamily, socketType );
		jobject clientNetworkAddress = (*env)->NewObject( env, IPv4Address_c, IPv4Address_m, ip );
		
		/* Create the connection endpoint object */
		jobject clientTransportEndPoint = (*env)->NewObject( env, Transport_c, Transport_m, clientNetworkAddress, native_port );
		
		/* Move over the fields */
		(*env)->SetObjectField( env, clientSocket, attachedHost_f, clientTransportEndPoint );
		(*env)->SetIntField( env, clientSocket, cSocket_f, native_clientSocketFD );
	}
	else {
		throw_notImplemented( env );
	}
	
	return clientSocket;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recv
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1recv ) ( JNIEnv *env, jobject obj, jint count ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	int err;
	char native_msg[count * sizeof(char) + 1];
	
	/* Zero our memory, eh? Good idea! */
	memset( native_msg, '\0', count * sizeof(char) + 1 );
	
	err = recv( cSocket, native_msg, count * sizeof(char), 0 );

	if ( err < 0 ) {
		xomios_error_throw( env, errno );
	}

	jstring msg = (*env)->NewStringUTF( env, native_msg );
	
	return msg;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recvfrom
 * Signature: (ILorg/xomios/connectivity/net/ConnectionEndPoint;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1recvfrom ) ( JNIEnv *env, jobject obj, jint count, jobject host ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Socket address family */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* Socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	int err;
	char native_msg[count * sizeof(char) + 1];
	
	/* Remote host */
	struct sockaddr_in native_rhost;
	socklen_t native_rhost_size = sizeof(struct sockaddr_in); 
	
	/* Zero our memory, eh? Good idea! */
	memset( native_msg, '\0', count * sizeof(char) + 1 );
	
	err = recvfrom( cSocket, native_msg, count * sizeof(char), 0, (struct sockaddr *) &native_rhost, &native_rhost_size );

	if ( err < 0 ) {
		xomios_error_throw( env, errno );
	}

	/* Message received that should be returned */
	jstring msg = (*env)->NewStringUTF( env, native_msg );
	
	jobject hostAddress;
	if ( addressFamily == XOM_NATIVE_INTERNAL_SOCKET_AF_INET ) {
		jclass IPv4Address_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/IPv4Address;" );
		jmethodID IPv4Address_m = (*env)->GetMethodID( env, IPv4Address_c, "<init>", "([B)V" );
		
		/* Get the IP as a Java byte array */
		ubyte native_ip[4];
		unsigned int native_ip_raw = ntohl( native_rhost.sin_addr.s_addr );
		native_ip[0] = (native_ip_raw >> 24 ) & 0xFF;
		native_ip[1] = (native_ip_raw >> 16 ) & 0xFF;
		native_ip[2] = (native_ip_raw >>  8 ) & 0xFF;
		native_ip[3] = native_ip_raw & 0xFF;
		
		jbyteArray ip = (*env)->NewByteArray( env, 4 );
		(*env)->SetByteArrayRegion( env, ip, 0, 4, (sbyte *) native_ip );
		
		/* Create the object */
		hostAddress = (*env)->NewObject( env, IPv4Address_c, IPv4Address_m, ip );
	}
	else {
		throw_notImplemented( env );
	}
	
	/* Now we need to build the ConnectionEndPoint object */
	if ( socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM || socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM ) {
		jclass ConnectionEndPoint_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/ConnectionEndPoint;" );
		jclass NetworkPort_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkPort;" );
		
		jmethodID NetworkPort_setPort_m = (*env)->GetMethodID( env, NetworkPort_c, "setPort", "(I)V" );
		jmethodID ConnectionEndPoint_setNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "setNetworkAddress", "(Lorg/xomios/connectivity/net/NetworkAddress;)V" );
		
		(*env)->CallVoidMethod( env, host, ConnectionEndPoint_setNetworkAddress_m, hostAddress );
		(*env)->CallVoidMethod( env, host, NetworkPort_setPort_m, ntohs(native_rhost.sin_port) );
	}
	else {
		throw_notImplemented( env );
	}
	
	return msg;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    send
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1send ) ( JNIEnv *env, jobject obj, jstring data ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );

	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Store return value of send call */
	int err, native_msglen;
	char* native_msg;
		
	/* Retreive the string */
	native_msg = (char*) (*env)->GetStringUTFChars( env, data, NULL );
	
	err = native_msglen = send( cSocket, native_msg, strlen(native_msg) + 1, 0 );
	if ( err < 0 ) {
		xomios_error_throw( env, errno );
	}
	
	/* Release memory allocated to store message */
	(*env)->ReleaseStringChars( env, data, (jchar*) native_msg );
	
	return native_msglen;
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    sendto
 * Signature: (Ljava/lang/String;Lorg/xomios/connectivity/net/ConnectionEndPoint;)I
 */
JNIEXPORT jint JNICALL XOM_NATIVE_INTERNAL_SOCKET( _1sendto ) ( JNIEnv *env, jobject obj, jstring data, jobject host ) {
	jclass Socket_c = (*env)->GetObjectClass( env, obj );
	jclass ConnectionEndPoint_c = (*env)->GetObjectClass( env, host );
	jclass NetworkAddress_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkAddress;" );
	
	/* Grab the socket file descriptor value */
	jfieldID cSocket_f = (*env)->GetFieldID( env, Socket_c, "cSocket", "I" );
	jint cSocket = (*env)->GetIntField( env, obj, cSocket_f );
	
	/* Socket address family */
	jfieldID addressFamily_f = (*env)->GetFieldID( env, Socket_c, "addressFamily", "I" );
	jint addressFamily = (*env)->GetIntField( env, obj, addressFamily_f );
	
	/* Socket type */
	jfieldID socketType_f = (*env)->GetFieldID( env, Socket_c, "socketType", "I" );
	jint socketType = (*env)->GetIntField( env, obj, socketType_f );
	
	/* ConnectionEndPoint methods */
	jmethodID ConnectionEndPoint_getNetworkAddress_m = (*env)->GetMethodID( env, ConnectionEndPoint_c, "getNetworkAddress", "()Lorg/xomios/connectivity/net/NetworkAddress;" );
	jobject hostAddress = (*env)->CallObjectMethod( env, host, ConnectionEndPoint_getNetworkAddress_m );
	
	/* Store return value of send call */
	int err, native_msglen;
		
	/* Retreive the string */
	char* native_msg = (char*) (*env)->GetStringUTFChars( env, data, NULL );
	
	if ( socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_STREAM || socketType == XOM_NATIVE_INTERNAL_SOCKET_SOCK_DGRAM ) {
		jclass NetworkPort_c = (*env)->FindClass( env, "Lorg/xomios/connectivity/net/NetworkPort;" );
		jmethodID NetworkPort_getPort_m = (*env)->GetMethodID( env, NetworkPort_c, "getPort", "()I" );
		
		jint port = (*env)->CallIntMethod( env, host, NetworkPort_getPort_m );
		
		if ( addressFamily == XOM_NATIVE_INTERNAL_SOCKET_AF_INET ) {
			jmethodID NetworkAddress_getAddress_m = (*env)->GetMethodID( env, NetworkAddress_c, "getAddress", "()[B" );
			jbyteArray ip = (*env)->CallObjectMethod( env, hostAddress, NetworkAddress_getAddress_m );			
			ubyte* native_ip = (ubyte *) (*env)->GetByteArrayElements( env, ip, NULL );

			struct sockaddr_in native_endpoint;
			native_endpoint.sin_family = getNativeAddressFamily( addressFamily );
			native_endpoint.sin_port = htons( port );
			native_endpoint.sin_addr.s_addr = htonl( ( native_ip[0] << 24 ) |
			                                         ( native_ip[1] << 16 ) |
			                                         ( native_ip[2] <<  8 ) | 
			                                         native_ip[3] );
			
			/* + 1 in size to include the \0 
			 * NOTE: This is stupid. A low level implementation should be sending byte[]'s not Strings
			 * TODO: Fix that ^ */
			err = native_msglen = sendto( cSocket, native_msg, strlen(native_msg) + 1, 0, (struct sockaddr *) &native_endpoint, sizeof(native_endpoint) );
			
			if ( err < 0 ) {
				xomios_error_throw( env, errno );
			}

			(*env)->ReleaseByteArrayElements( env, ip, (sbyte *) native_ip, 0 );
		}
		else {
			throw_notImplemented( env );
		}
	}
	else {
		throw_notImplemented( env );
	}
	
	/* Release memory allocated to store message */
	(*env)->ReleaseStringChars( env, data, (jchar*) native_msg );
	
	return native_msglen;
}
