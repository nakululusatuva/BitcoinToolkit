AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CFLAGS = -Wall
noinst_PROGRAMS = test
test_LDADD = /usr/lib/x86_64-linux-gnu/libcheck_pic.a
test_SOURCES = test.c \
	CStack_check.c \
	CLinkedlist_check.c \
	../src/container/CStack.c \
	../src/container/CLinkedlist.c