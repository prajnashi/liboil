
pkgincludedir = $(includedir)/liboil-@LIBOIL_MAJORMINOR@/liboil

SUBDIRS = copy dct jpeg simdpack

lib_LTLIBRARIES = liboil-@LIBOIL_MAJORMINOR@.la
noinst_LTLIBRARIES = liboilend.la liboiltmp.la

noinst_PROGRAMS = build_prototypes build_lists

pkginclude_HEADERS = liboil.h liboilfunction.h liboildebug.h liboilfuncs.h \
	liboiltypes.h liboilcpu.h

liboilend_la_SOURCES = \
	liboil_end.c
liboilend_la_CFLAGS = $(LIBOIL_CFLAGS)
liboilend_la_LDFLAGS = 

if HAVE_CPU_POWERPC
powerpc_sources = conv_powerpc.c
else
powerpc_sources = 
endif

if HAVE_CPU_I386
i386_sources = conv_sse.c
else
i386_sources = 
endif

if HAVE_GNU_LINKER
array_sources = 
else
array_sources = liboilarray.c
endif

liboiltmp_la_SOURCES = \
	conv.h \
	liboil.h \
	liboildebug.h \
	liboilfuncs.h \
	liboilfunction.h \
	liboilprofile.h \
	liboiltypes.h \
	conv_ref.c \
	conv_c.c \
	conv_bitstuff.c \
	conv_misc.c \
	$(powerpc_sources) \
	$(i386_sources) \
	liboilfunction.c \
	liboildebug.c \
	liboilcpu.c \
	liboilprototype.c \
	$(array_sources)
liboiltmp_la_CFLAGS = $(LIBOIL_CFLAGS)
liboiltmp_la_LDFLAGS = -lm

liboil_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	liboil_begin.c
liboil_@LIBOIL_MAJORMINOR@_la_LIBADD = \
	liboiltmp.la \
	copy/libcopy.la \
	dct/libdct.la \
	jpeg/libjpeg.la \
	simdpack/libsimdpack.la \
	liboilend.la
liboil_@LIBOIL_MAJORMINOR@_la_CFLAGS = $(LIBOIL_CFLAGS)
liboil_@LIBOIL_MAJORMINOR@_la_LDFLAGS = -lm \
	-export-symbols-regex '^oil_'

build_prototypes_SOURCES = build_prototypes.c
build_prototypes_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)
build_prototypes_LDFLAGS = $(LIBOIL_LIBS) $(GLIB_LIBS)

build_lists_SOURCES = build_lists.c
build_lists_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)
build_lists_LDFLAGS = $(LIBOIL_LIBS) $(GLIB_LIBS)

