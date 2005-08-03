
CLEANFILES = empty.c

if USE_ALT_OPT
opt_libs = libfb_opt.la
else
opt_libs =
endif
noinst_LTLIBRARIES = libfb.la $(opt_libs)

if HAVE_CPU_I386
c_sources = \
	fbmmx.c
else
if HAVE_CPU_AMD64
c_sources = \
	fbmmx.c
else
c_sources = \
	empty.c
endif
endif

noinst_HEADERS = \
	fbmmx.h \
	fbpict.h

libfb_la_SOURCES = $(c_sources)
libfb_la_CFLAGS = $(MMX_CFLAGS) $(SSE_CFLAGS) $(LIBOIL_CFLAGS)

libfb_opt_la_SOURCES = $(c_sources)
libfb_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS) $(MMX_CFLAGS) $(SSE_CFLAGS)

empty.c:
	echo >empty.c

