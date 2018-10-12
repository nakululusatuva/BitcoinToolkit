AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -fPIC -shared -O3 -Iinclude/

projectlibdir = /usr/lib
includedir = /usr/include/bitcointk
projectlib_PROGRAMS = libbitcointk.so

libbitcointk_so_SOURCES = src/address.c \
	src/crypto/bigint.c \
	src/codec/base.c \
	src/codec/strings.c \
	src/container/CStack.c \
	src/container/CLinkedlist.c \
	src/machine/script.c \
	src/machine/interpreter.c \
	src/machine/operation.c
include_HEADERS = include/internal/common.h