# Xomios Project
# Copyright (c) 2007 Xomios Contributors
#
# This file is released under a modified BSD license. For more information,
# see the LICENSE file included in this distribution.

# Makefile for POSIX systems.

# This file (and all included files) must be compatible with GNU and BSD make.

OBJECT_PATH = .libs

OBJECTS = $(OBJECT_PATH)/native_internal_Socket.o \
	$(OBJECT_PATH)/native_internal_File.o \
	$(OBJECT_PATH)/xomios_error.o
OBJECT_SHARED = libxomios.so
SOURCES = native_internal_Socket.c native_internal_File.c xomios_error.c

JAVA_INCLUDE = $(JAVA_PATH)/include
JAVA_INCLUDE_PLATFORM = `find "$(JAVA_INCLUDE)" -name "jni_md.h" -exec dirname "{}" \; | head -n 1`

CFLAGS += -pedantic -std=c99 -Wall -Wundef -Wmissing-prototypes -Wmissing-declarations -Werror -fPIC -D _LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_XOPEN_SOURCE=600 -I. -I../../../native -I$(JAVA_INCLUDE) -I$(JAVA_INCLUDE_PLATFORM)
CFLAGS_LIBRARY = $(CFLAGS) -shared

.PHONY: all all-prepare all-linux clean

all: all-prepare $(OBJECTS) $(OBJECT_SHARED)

all-prepare:
	@if test ! -f "$(JAVA_INCLUDE)/jni.h"; then \
		echo "Invalid Java include path \"$(JAVA_INCLUDE)\" (JAVA_PATH = \"$(JAVA_PATH)\"); check paths"; \
		exit 1; \
	fi
	@if test ! -f "$(JAVA_INCLUDE_PLATFORM)/jni_md.h"; then \
		echo "Invalid Java platform include path \"$(JAVA_INCLUDE_PLATFORM)\" (JAVA_PATH = \"$(JAVA_PATH)\"); check paths"; \
		exit 1; \
	fi
	@echo "JAVA_PATH = $(JAVA_PATH)"
	@echo "JAVA_INCLUDE = $(JAVA_INCLUDE)"
	@echo "JAVA_INCLUDE_PLATFORM = ${JAVA_INCLUDE_PLATFORM}"
	-mkdir -p $(OBJECT_PATH)

install: all install-prepare
	-mkdir -p $(INSTALL_PATH)
	install -p --mode=0755 $(OBJECT_SHARED) $(INSTALL_PATH)

install-prepare:
	@if test -z "$(INSTALL_PATH)"; then \
		echo "Installation path (INSTALL_PATH) is not defined"; \
		exit 1; \
	fi
	@echo "INSTALL_PATH = $(INSTALL_PATH)"

$(OBJECT_PATH)/native_internal_Socket.o: native_internal_Socket.c
	$(CC) $(CFLAGS) -c \
		-o $(OBJECT_PATH)/native_internal_Socket.o \
		native_internal_Socket.c

$(OBJECT_PATH)/native_internal_File.o: native_internal_File.c
	$(CC) $(CFLAGS) -c \
		-o $(OBJECT_PATH)/native_internal_File.o \
		native_internal_File.c

$(OBJECT_PATH)/xomios_error.o: xomios_error.c
	$(CC) $(CFLAGS) -c \
		-o $(OBJECT_PATH)/xomios_error.o \
		xomios_error.c

$(OBJECT_SHARED):
	$(CC) $(CFLAGS_LIBRARY) -o $(OBJECT_SHARED) $(OBJECTS)

$(SOURCES):

clean:
	-$(RM) -r $(OBJECT_PATH)
	-$(RM) $(OBJECT_SHARED)
