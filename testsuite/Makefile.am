
SUBDIRS = instruction

programs = align moo introspect proto1 proto2 test1 proto3 proto4 stride \
	dso_check abs md5 md5_profile trans copy zigzag

check_PROGRAMS = $(programs)

TESTS = $(programs) list_impls

AM_LDFLAGS = $(LIBOIL_LIBS)
AM_CFLAGS = $(LIBOIL_CFLAGS)

