
noinst_LTLIBRARIES = libmotovec.la

c_sources = 

if HAVE_GCC_POWERPC
powerpc_sources = \
	motovec.c \
	vec_memcpy.S \
	vec_memset.S
else
powerpc_sources =
endif

libmotovec_la_SOURCES = 	\
	$(powerpc_sources)
libmotovec_la_LIBADD = 
libmotovec_la_CFLAGS = $(LIBOIL_CFLAGS)
libmotovec_la_CCASFLAGS = $(LIBOIL_CFLAGS)

