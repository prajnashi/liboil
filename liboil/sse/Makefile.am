
if USE_ALT_OPT
opt_libs = libsse_opt.la
else
opt_libs =
endif
noinst_LTLIBRARIES = libsse.la $(opt_libs)

if HAVE_CPU_I386
c_sources = \
	conv_sse.c
endif

libsse_la_SOURCES = $(c_sources)
libsse_la_CFLAGS = $(MMX_CFLAGS) $(SSE_CFLAGS) $(SSE2_CFLAGS) $(LIBOIL_CFLAGS)

libsse_opt_la_SOURCES = $(c_sources)
libsse_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS) $(MMX_CFLAGS) $(SSE_CFLAGS) $(SSE2_CFLAGS)

libsse_la_SOURCES = \
	$(c_sources)

