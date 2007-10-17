# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

# Makefile for Linux x86 32-bit architecture

OBJECTS=internal_Socket.o
SOURCES=internal_Socket.c

CC=gcc
CFLAGS+=-Wall -shared -Wl -static -lc
JVM_INCLUDE=-I/usr/lib/jvm/java-6-sun/include/

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o libxomios.so -Wl,-soname,libxomios.so $(OBJECTS)

internal_Socket.o: internal_Socket.c
	$(CC) $(CFLAGS) $(JVM_INCLUDE) -o internal_Socket.o internal_Socket.c

$(SOURCES):

.PHONY: clean
clean:
	-rm $(OBJECTS) libxomios.so
