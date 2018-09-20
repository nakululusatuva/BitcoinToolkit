AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall -fPIC -shared -O3
noinst_PROGRAMS = libbtctool.so
libbtctool_so_SOURCES = src/status.c \
	src/address.c \
	src/cJSON/cJSON.c \
	src/codec/base.c \
	src/codec/strings.c \
	src/container/CStack.c \
	src/container/CLinkedlist.c \
	src/script/script.c \
	src/script/interpreter.c \
	src/script/operation.c
