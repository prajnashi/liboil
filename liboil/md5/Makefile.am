
if USE_ALT_OPT
opt_libs = libmd5_opt.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libmd5.la $(opt_libs)

noinst_HEADERS = md5.h

if HAVE_CPU_I386
i386_sources = md5_i386.c
else
i386_sources =
endif

if HAVE_CPU_POWERPC
powerpc_sources = md5_powerpc.c
else
powerpc_sources =
endif

libmd5_la_SOURCES = \
	md5.c $(i386_sources) $(powerpc_sources)
libmd5_la_LIBADD = $(opt_libs)
libmd5_la_CFLAGS = $(LIBOIL_CFLAGS)

libmd5_opt_la_SOURCES = md5
libmd5_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

