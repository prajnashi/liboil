
includedir = $(includedir)/orc-@LIBOIL_MAJORMINOR@/orc

lib_LTLIBRARIES = liborc-@LIBOIL_MAJORMINOR@.la

liborc_@LIBOIL_MAJORMINOR@_la_LIBS = $(LIBOIL_LIBS) $(GLIB_LIBS)
liborc_@LIBOIL_MAJORMINOR@_la_CFLAGS = $(LIBOIL_CFLAGS) $(GLIB_CFLAGS)

liborc_@LIBOIL_MAJORMINOR@_la_SOURCES = \
	orcexecutor.c \
	orcrule.c \
	orctype.c \
	ojprogram.c \
	ojprogram-x86.c \
	ojprogram.h \
	ojopcodes.c \
	ojprogram-linux.c

