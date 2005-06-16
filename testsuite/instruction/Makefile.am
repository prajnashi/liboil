
EXTRA_DIST = check-instructions.pl

noinst_PROGRAMS = list-impls

noinst_DATA = report

AM_LDFLAGS = $(LIBOIL_LIBS)
AM_CFLAGS = $(LIBOIL_CFLAGS)
	

report: list-impls $(srcdir)/check-instructions.pl
	$(srcdir)/check-instructions.pl $(top_builddir)/liboil/.libs/liboiltmp1.so >report


CLEANFILES = report

