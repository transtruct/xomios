#!/bin/sh

LD_LIBRARY_PATH=../build/linux/x86_32/lib/ CLASSPATH=.:../build/linux/x86_32/classes/ java $@
