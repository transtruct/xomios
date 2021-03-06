/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _XOMIOS_ERROR_H
#define _XOMIOS_ERROR_H

#include "xomios_platform.h"

#include "xomios.h"

#include <errno.h>
#include <string.h>

#ifndef ENOERR
# define ENOERR 0
#endif

void xomios_error_throw ( JNIEnv *, int );

#endif
