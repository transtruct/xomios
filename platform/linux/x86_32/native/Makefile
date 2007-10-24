# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

# Makefile for Linux x86 32-bit architecture

OBJECTS = internal_Socket.o internal_File.o xomios_error.o
SOURCES = internal_Socket.c internal_File.c xomios_error.c

CFLAGS += -pedantic -std=c99 -Wall -fPIC -I/usr/lib/jvm/java-6-sun/include/ -I/usr/lib/jvm/java-6-sun/include/linux/
LIBS = -pedantic -std=c99 -Wall -shared -fPIC

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(LIBS) -o libxomios.so $(OBJECTS)

xomios_error.o: xomios_error.c
	$(CC) $(CFLAGS) -o xomios_error.o -c xomios_error.c

internal_Socket.o: internal_Socket.c
	$(CC) $(CFLAGS) -o internal_Socket.o -c internal_Socket.c

internal_File.o: internal_File.c
	$(CC) $(CFLAGS) -o internal_File.o -c internal_File.c

javah: 
	javah -jni -classpath ../../../../build/linux/x86_32/classes/ org.xomios.internal.Socket
	mv org_xomios_internal_Socket.h internal_Socket.h

$(SOURCES):

clean:
	-rm $(OBJECTS)
