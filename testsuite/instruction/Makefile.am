
EXTRA_DIST = check-instructions.pl

check_PROGRAMS = list-impls

if HAVE_CPU_I386
#check_DATA = report
else
#check_DATA = 
endif

AM_LDFLAGS = $(LIBOIL_LIBS)
AM_CFLAGS = $(LIBOIL_CFLAGS)
	

report: list-impls $(srcdir)/check-instructions.pl $(top_builddir)/liboil/.libs/liboiltmp1.so
	$(srcdir)/check-instructions.pl $(top_builddir)/liboil/.libs/liboiltmp1.so >report


CLEANFILES = report

