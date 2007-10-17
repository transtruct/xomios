/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#include "internel_Socket.h"
#include <sys/socket.h>

/*
 * Class:     org_xomios_internal_Socket
 * Method:    createSocket
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_createSocket ( JNIEnv *env, jobject obj ) {
	jfieldid socketdf = (*env)->getFieldID( env, "csocket", "I" );
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_xomios_internal_Socket_close ( JNIEnv *env, jobject obj ) {
	
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    recv
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_xomios_internal_Socket_recv ( JNIEnv *env, jobject obj, jint count ) {
	
}

/*
 * Class:     org_xomios_internal_Socket
 * Method:    send
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_xomios_internal_Socket_send ( JNIEnv *env, jobject obj, jstring data ) {
	
}


