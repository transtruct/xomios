# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

# Makefile for Linux x86 32-bit architecture

OBJECTS = native_internal_Socket.o native_internal_File.o xomios_error.o
SOURCES = native_internal_Socket.c native_internal_File.c xomios_error.c

CFLAGS += -pedantic -std=c99 -Wall -fPIC -I. -I/usr/lib/jvm/java-6-sun/include/ -I/usr/lib/jvm/java-6-sun/include/linux/
LIBS = -pedantic -std=c99 -Wall -shared -fPIC

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(LIBS) -o libxomios.so $(OBJECTS)

xomios_error.o: xomios_error.c
	$(CC) $(CFLAGS) -o xomios_error.o -c xomios_error.c

internal_Socket.o: native_internal_Socket.c
	$(CC) $(CFLAGS) -o native_internal_Socket.o -c native_internal_Socket.c

internal_File.o: native_internal_File.c
	$(CC) $(CFLAGS) -o native_internal_File.o -c native_internal_File.c

$(SOURCES):

clean:
	-rm $(OBJECTS)
