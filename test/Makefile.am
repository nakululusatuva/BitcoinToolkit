AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall
noinst_PROGRAMS = test
test_LDADD = /usr/lib/x86_64-linux-gnu/libcheck_pic.a
test_SOURCES = main.c \
	src/CStack_check.c \
	src/CLinkedlist_check.c \
	src/Script_check.c \
	../src/container/CStack.c \
	../src/container/CLinkedlist.c \
	../src/script/script.c \
	../src/codec/strings.c
