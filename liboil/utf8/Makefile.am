
if USE_ALT_OPT
opt_libs = libutf8_opt.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libutf8.la $(opt_libs)

noinst_HEADERS = utf8.h

c_sources = utf8.c utf8_fast.c

if HAVE_CPU_I386
#i386_sources = utf8_i386.c
else
#i386_sources =
endif

if HAVE_CPU_POWERPC
#powerpc_sources = utf8_powerpc.c
else
#powerpc_sources =
endif

libutf8_la_SOURCES = \
	$(c_sources) $(i386_sources) $(powerpc_sources)
libutf8_la_LIBADD = $(opt_libs)
libutf8_la_CFLAGS = $(LIBOIL_CFLAGS)

libutf8_opt_la_SOURCES = $(c_sources)
libutf8_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

