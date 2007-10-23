/*
 * Xomios Project
 * Copyright (c) 2007 Xomios Contributors
 * 
 * This file is released under a modified BSD license. For more information,
 * see the LICENSE file included in this distribution.
 */

#ifndef _XOMIOS_PLATFORM_H
#define _XOMIOS_PLATFORM_H

#if defined(__linux__)
# define XOM_PLATFORM_LINUX
# if defined(__gnu_linux__)
#  define XOM_PLATFORM_GNU_LINUX
# endif
#elif defined(__alpha__)
# define XOM_PLATFORM_ALPHA
#elif (defined(__sun__) || defined(sun))
# define XOM_PLATFORM_SUNOS
#elif defined(__hpux__)
# define XOM_PLATFORM_HPUX
#endif

#endif
