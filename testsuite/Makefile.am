
if HAVE_GLIB
glib_programs = abs md5 md5_profile trans copy
else
glib_programs =
endif

programs = moo introspect proto1 proto2 test1 proto3 stride

check_PROGRAMS = $(programs) $(glib_programs)

TESTS = $(programs) $(glib_programs)

AM_LDFLAGS = $(LIBOIL_LIBS) $(GLIB_LIBS)
AM_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)

