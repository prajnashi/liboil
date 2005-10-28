
CLEANFILES = empty.c

if USE_ALT_OPT
opt_libs = libsse_opt.la
else
opt_libs =
endif
noinst_LTLIBRARIES = libsse.la $(opt_libs)

if HAVE_GCC_I386
c_sources = \
	conv_sse.c
else
if HAVE_GCC_AMD64
c_sources = \
	conv_sse.c
else
c_sources = \
	empty.c
endif
endif

libsse_la_SOURCES = $(c_sources)
libsse_la_CFLAGS = $(SSE_CFLAGS) $(SSE2_CFLAGS) $(LIBOIL_CFLAGS)

libsse_opt_la_SOURCES = $(c_sources)
libsse_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS) $(MMX_CFLAGS) $(SSE_CFLAGS) $(SSE2_CFLAGS)

empty.c:
	echo >empty.c

