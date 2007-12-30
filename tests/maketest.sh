#!/bin/sh

LD_LIBRARY_PATH=../build/posix/linux-i686/lib/ CLASSPATH=.:../build/posix/linux-i686/classes/ javac $@
