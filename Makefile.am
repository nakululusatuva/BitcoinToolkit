AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -fPIC -shared -O3
noinst_PROGRAMS = libbtctool.so
libbtctool_so_SOURCES = src/status.c \
	src/address.c \
	src/codec/base.c \
	src/codec/strings.c \
	src/container/CStack.c \
	src/container/CLinkedlist.c \
	src/machine/script.c \
	src/machine/interpreter.c \
	src/machine/operation.c
