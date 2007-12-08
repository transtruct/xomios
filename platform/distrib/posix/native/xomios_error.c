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
 * Array of error numbers and corresponding Java classes. These are defined by
 * IEEE 1003.1, issue 6.
 */
static const char *const __error_handlers[] = {
	/* java.lang.Error */
	[EFAULT] = "Ljava/lang/Error;",
	
	/* org.xomios.AccessException */
	[EACCES] = "Lorg/xomios/AccessException;",
	[EPERM] = "Lorg/xomios/AccessException;",
	[EROFS] = "Lorg/xomios/AccessException;",
	
	/* java.lang.Exception */
	[EILSEQ] = "Ljava/lang/Exception;", /* Temporary. */
	
	/* org.xomios.connectivity.ConnectivityException */
	[EALREADY] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ECONNABORTED] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ECONNREFUSED] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ECONNRESET] = "Lorg/xomios/connectivity/ConnectivityException;",
	[EDESTADDRREQ] = "Lorg/xomios/connectivity/ConnectivityException;",
	[EHOSTUNREACH] = "Lorg/xomios/connectivity/ConnectivityException;",
	[EISCONN] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENETDOWN] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENETRESET] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENETUNREACH] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENOPROTOOPT] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENOTCONN] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ENOTSOCK] = "Lorg/xomios/connectivity/ConnectivityException;",
	[EPIPE] = "Lorg/xomios/connectivity/ConnectivityException;",
	[ESPIPE] = "Lorg/xomios/connectivity/ConnectivityException;",
#if defined( ETIME )
	[ETIME] = "Lorg/xomios/connectivity/ConnectivityException;",
#endif
	[ETIMEDOUT] = "Lorg/xomios/connectivity/ConnectivityException;",
	
	/* org.xomios.IOException */
	[EBADF] = "Lorg/xomios/IOException;",
	[EBADMSG] = "Lorg/xomios/IOException;",
	[EIO] = "Lorg/xomios/IOException;",
	[ENODATA] = "Lorg/xomios/IOException;",
	[EPROTO] = "Lorg/xomios/IOException;",
	
	/* java.lang.IllegalArgumentException */
	[EDOM] = "Ljava/lang/IllegalArgumentException;",
	
	/* org.xomios.IllegalOperationException */
#if defined( ENOSTR )
	[ENOSTR] = "Lorg/xomios/IllegalOperationException;",
#endif
	[ENOTTY] = "Lorg/xomios/IllegalOperationException;",
	[EPROTOTYPE] = "Lorg/xomios/IllegalOperationException;",
	
	/* org.xomios.location.InvalidLocationException */
	[EADDRNOTAVAIL] = "Lorg/xomios/location/InvalidLocationException;",
	[EEXIST] = "Lorg/xomios/location/InvalidLocationException;",
	[EISDIR] = "Lorg/xomios/location/InvalidLocationException;",
	[ELOOP] = "Lorg/xomios/location/InvalidLocationException;",
	[ENAMETOOLONG] = "Lorg/xomios/location/InvalidLocationException;",
	[ENODEV] = "Lorg/xomios/location/InvalidLocationException;",
	[ENOENT] = "Lorg/xomios/location/InvalidLocationException;",
	[ENOTDIR] = "Lorg/xomios/location/InvalidLocationException;",
	[ENOTEMPTY] = "Lorg/xomios/location/InvalidLocationException;",
	[ENXIO] = "Lorg/xomios/location/InvalidLocationException;",
	[ESRCH] = "Lorg/xomios/location/InvalidLocationException;",
	[EXDEV] = "Lorg/xomios/location/InvalidLocationException;",
	
	/* org.xomios.OverflowException */
	[EFBIG] = "Lorg/xomios/OverflowException;",
	[EMSGSIZE] = "Lorg/xomios/OverflowException;",
	[EOVERFLOW] = "Lorg/xomios/OverflowException;",
	[ERANGE] = "Lorg/xomios/OverflowException;",
	
	/* org.xomios.ProcessException */
	[ECANCELED] = "Lorg/xomios/ProcessException;",
	[ECHILD] = "Lorg/xomios/ProcessException;",
	[EIDRM] = "Lorg/xomios/ProcessException;",
	[EINPROGRESS] = "Lorg/xomios/ProcessException;",
	[EINTR] = "Lorg/xomios/ProcessException;",
	[ENOMSG] = "Lorg/xomios/ProcessException;",
	
	/* org.xomios.ResourceException */
	[EAGAIN] = "Lorg/xomios/ResourceException;",
	[EBUSY] = "Lorg/xomios/ResourceException;",
	[ENOEXEC] = "Lorg/xomios/ResourceException;",
	[ETXTBSY] = "Lorg/xomios/ResourceException;",
	[EWOULDBLOCK] = "Lorg/xomios/ResourceException;",
	
	/* org.xomios.ResourceAllocationException */
	[E2BIG] = "Lorg/xomios/ResourceAllocationException;",
	[EADDRINUSE] = "Lorg/xomios/ResourceAllocationException;",
	
	/* org.xomios.ResourceLimitException */
	[EDEADLK] = "Lorg/xomios/ResourceLimitException;",
	[EMFILE] = "Lorg/xomios/ResourceLimitException;",
	[EMLINK] = "Lorg/xomios/ResourceLimitException;",
	[ENFILE] = "Lorg/xomios/ResourceLimitException;",
	[ENOBUFS] = "Lorg/xomios/ResourceLimitException;",
	[ENOLCK] = "Lorg/xomios/ResourceLimitException;",
	[ENOMEM] = "Lorg/xomios/ResourceLimitException;",
	[ENOSPC] = "Lorg/xomios/ResourceLimitException;",
#if defined( ENOSR )
	[ENOSR] = "Lorg/xomios/ResourceLimitException;",
#endif
	
	/* java.lang.UnsupportedOperationException */
	[EAFNOSUPPORT] = "Ljava/lang/UnsupportedOperationException;",
	[ENOSYS] = "Ljava/lang/UnsupportedOperationException;",
	[ENOTSUP] = "Ljava/lang/UnsupportedOperationException;",
	[EOPNOTSUPP] = "Ljava/lang/UnsupportedOperationException;",
	[EPROTONOSUPPORT] = "Ljava/lang/UnsupportedOperationException;",
	
	/* java.lang.Exception */
	/* The following error codes are reserved and are not currently used by
	 * implementations. */
	[EDQUOT] = "Ljava/lang/Exception;",
	[EMULTIHOP] = "Ljava/lang/Exception;",
	[ENOLINK] = "Ljava/lang/Exception;",
	[ESTALE] = "Ljava/lang/Exception;"
};

static const int __error_handlers_size = sizeof(__error_handlers) / sizeof(__error_handlers[EFAULT]);

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
	
	/* Grab the current error string. */
	char buffer[__ERROR_BUFFER_SIZE];
	strerror_r( error, buffer, __ERROR_BUFFER_SIZE - 1 );
	
	jclass c_Exception;
	
	if (error >= __error_handlers_size || error < 0 || __error_handlers[error] == NULL) {
		/* No match, so throw a default exception. */
		c_Exception = (*env)->FindClass( env, "Ljava/lang/Exception;" );
	}
	else {
		c_Exception = (*env)->FindClass( env, __error_handlers[error] );
	}
	
	(*env)->ThrowNew( env, c_Exception, buffer );
}
