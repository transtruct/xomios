/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "xomios_error.h"

#define __ERROR_BUFFER_SIZE 256

/**
 * Throws an exception based on the POSIX error code passed.
 * 
 * @param env The JNI environment.
 * @param error The error code.
 */
void xomios_error_throw ( JNIEnv *env, int error ) {
	if ( error == ENOERR ) {
		return;
	}
	
	jclass c_Exception;
	
	switch ( error ) {
	/* java.lang.Error */
	case EFAULT:
		c_Exception = (*env)->FindClass( env, "Ljava/lang/Error;" );
		break;
	
	/* org.xomios.AccessException */
	case EACCES:
	case EPERM:
	case EROFS:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/AccessException" );
		break;
	
	/* java.lang.Exception */
	case EILSEQ:
		/* Temporary. */
		c_Exception = (*env)->FindClass( env, "Ljava/lang/Exception;" );
		break;
	
	/* org.xomios.connectivity.ConnectivityException */
	case EALREADY:
	case ECONNABORTED:
	case ECONNREFUSED:
	case ECONNRESET:
	case EDESTADDRREQ:
	case EHOSTUNREACH:
	case EISCONN:
	case ENETDOWN:
	case ENETRESET:
	case ENETUNREACH:
	case ENOPROTOOPT:
	case ENOTCONN:
	case ENOTSOCK:
	case EPIPE:
	case ESPIPE:
#if defined( ETIME )
	case ETIME:
#endif
	case ETIMEDOUT:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/connectivity/ConnectivityException;" );
		break;
	
	/* org.xomios.IOException */
	case EBADF:
	case EBADMSG:
	case EIO:
	case ENODATA:
	case EPROTO:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/IOException;" );
		break;
	
	/* java.lang.IllegalArgumentException */
	case EDOM:
		c_Exception = (*env)->FindClass( env, "Ljava/lang/IllegalArgumentException;" );
		break;
	
	/* org.xomios.IllegalOperationException */
#if defined( ENOSTR )
	case ENOSTR:
#endif
	case ENOTTY:
	case EPROTOTYPE:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/IllegalOperationException;" );
		break;
	
	/* org.xomios.location.InvalidLocationException */
	case EADDRNOTAVAIL:
	case EEXIST:
	case EISDIR:
	case ELOOP:
	case ENAMETOOLONG:
	case ENODEV:
	case ENOENT:
	case ENOTDIR:
	case ENOTEMPTY:
	case ENXIO:
	case ESRCH:
	case EXDEV:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/location/InvalidLocationException;" );
		break;
	
	/* org.xomios.OverflowException */
	case EFBIG:
	case EMSGSIZE:
	case EOVERFLOW:
	case ERANGE:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/OverflowException;" );
		break;
	
	/* org.xomios.ProcessException */
	case ECANCELED:
	case ECHILD:
	case EIDRM:
	case EINPROGRESS:
	case EINTR:
	case ENOMSG:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/ProcessException;" );
		break;
	
	/* org.xomios.ResourceException */
	case EAGAIN:
	case EBUSY:
	case ENOEXEC:
	case ETXTBSY:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
	case EWOULDBLOCK:
#endif
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/ResourceException;" );
		break;
	
	/* org.xomios.ResourceAllocationException */
	case E2BIG:
	case EADDRINUSE:
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/ResourceAllocationException;" );
		break;
	
	/* org.xomios.ResourceLimitException */
	case EDEADLK:
	case EMFILE:
	case EMLINK:
	case ENFILE:
	case ENOBUFS:
	case ENOLCK:
	case ENOMEM:
	case ENOSPC:
#if defined( ENOSR )
	case ENOSR:
#endif
		c_Exception = (*env)->FindClass( env, "Lorg/xomios/ResourceLimitException;" );
		break;
	
	/* java.lang.UnsupportedOperationException */
	case EAFNOSUPPORT:
	case ENOSYS:
	case ENOTSUP:
#if EOPNOTSUPP != ENOTSUP
	case EOPNOTSUPP:
#endif
	case EPROTONOSUPPORT:
		c_Exception = (*env)->FindClass( env, "Ljava/lang/UnsupportedOperationException;" );
		break;
	
	/* java.lang.Exception */
	/* The following error codes are reserved and are not currently used by
	 * implementations. */
	case EDQUOT:
	case EMULTIHOP:
	case ENOLINK:
	case ESTALE:
	default:
		c_Exception = (*env)->FindClass( env, "Ljava/lang/Exception;" );
		break;
	}
	
	/* Grab the current error string. */
#if 0
	char buffer[__ERROR_BUFFER_SIZE];
	strerror_r( error, buffer, __ERROR_BUFFER_SIZE - 1 );
#else
	char *buffer;
	buffer = strerror_r( error, buffer, __ERROR_BUFFER_SIZE - 1 );
#endif
	
	/* Throw the exception. */
	(*env)->ThrowNew( env, c_Exception, buffer );
}
