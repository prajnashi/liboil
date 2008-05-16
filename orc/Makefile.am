
includedir = $(includedir)/orc-@LIBOIL_MAJORMINOR@/orc

lib_LTLIBRARIES = liborc-@LIBOIL_MAJORMINOR@.la

liborc_@LIBOIL_MAJORMINOR@_la_LIBS = $(LIBOIL_LIBS) $(GLIB_LIBS)
liborc_@LIBOIL_MAJORMINOR@_la_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)

liborc_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	orc.c \
	orcexecutor.c \
	orcrule.c \
	orctype.c \
	orcprogram.c \
	orcprogram-x86.c \
	orcprogram.h \
	orcopcodes.c \
	orcprogram-linux.c

