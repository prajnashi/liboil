
noinst_LTLIBRARIES = libmotovec.la

c_sources = 

if HAVE_CPU_POWERPC
powerpc_sources = \
	vec_memcpy.S
else
powerpc_sources =
endif

libmotovec_la_SOURCES = 	\
	$(powerpc_sources)
libmotovec_la_LIBADD = 
libmotovec_la_CFLAGS = $(LIBOIL_CFLAGS)

