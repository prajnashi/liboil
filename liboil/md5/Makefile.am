
noinst_LTLIBRARIES = libmd5.la

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

noinst_HEADERS = md5.h

libmd5_la_CFLAGS = $(LIBOIL_CFLAGS)


