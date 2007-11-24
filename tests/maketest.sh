#!/bin/sh

LD_LIBRARY_PATH=../build/posix/Linux_i386/lib/ CLASSPATH=.:../build/posix/Linux_i386/classes/ javac $@
