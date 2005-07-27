
if USE_ALT_OPT
opt_libs = libmath_opt1.la
else
opt_libs = 
endif
noinst_LTLIBRARIES = libmath.la $(opt_libs)

c_sources = \
	math.c

if HAVE_CPU_POWERPC
powerpc_sources =
else
powerpc_sources =
endif

if HAVE_CPU_I386
i386_sources =
else
i386_sources =
endif

libmath_la_SOURCES = 	\
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources)
libmath_la_LIBADD = $(opt_libs)
libmath_la_CFLAGS = $(LIBOIL_CFLAGS)

libmath_opt1_la_SOURCES = $(c_sources)
libmath_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

