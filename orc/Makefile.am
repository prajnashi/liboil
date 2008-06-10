
pkgincludedir = $(includedir)/orc-@LIBOIL_MAJORMINOR@/orc

lib_LTLIBRARIES = liborc-@LIBOIL_MAJORMINOR@.la

liborc_@LIBOIL_MAJORMINOR@_la_LIBS = $(LIBOIL_LIBS) $(GLIB_LIBS)
liborc_@LIBOIL_MAJORMINOR@_la_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)
liborc_@LIBOIL_MAJORMINOR@_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LIBOIL_LIBVERSION) \
	-export-symbols-regex '^orc_'


liborc_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	orc.c \
	orcexecutor.c \
	orcrule.c \
	orctype.c \
	orcprogram.c \
	orcprogram-c.c \
	orcprogram-x86.c \
	orcprogram-powerpc.c \
	orcprogram.h \
	orcopcodes.c \
	orcrules-mmx.c \
	orcrules-x86.c \
	orcrules-sse.c \
	x86.c

if HAVE_OS_LINUX
liborc_@LIBOIL_MAJORMINOR@_la_SOURCES += orcprogram-linux.c
else
liborc_@LIBOIL_MAJORMINOR@_la_SOURCES += orcprogram-unknown-os.c
endif


pkginclude_HEADERS = \
	orc.h \
	orcprogram.h \
	x86.h

