# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

# Makefile for Linux x86 32-bit architecture

OBJECTS = internal_Socket.o
SOURCES = internal_Socket.c

CFLAGS += -Wall -fPIC -I/usr/lib/jvm/java-6-sun/include/ -I/usr/lib/jvm/java-6-sun/include/linux/
LIBS = -Wall -shared -fPIC

all: $(OBJECTS)
	$(CC) $(LIBS) -o libxomios.so $(OBJECTS)

internal_Socket.o: internal_Socket.c
	$(CC) $(CFLAGS) -o internal_Socket.o -c internal_Socket.c

javah: 
	javah -jni -classpath ../../../../build/linux/x86_32/classes/ org.xomios.internal.Socket
	mv org_xomios_internal_Socket.h internal_Socket.h

$(SOURCES):

.PHONY: clean
clean:
	-rm $(OBJECTS)
