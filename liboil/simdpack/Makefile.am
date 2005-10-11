
if USE_ALT_OPT
opt_libs = libsimdpack_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libsimdpack.la $(opt_libs)

c_sources = \
	abs_misc.c \
	average2_u8.c \
	clip_fast.c \
	diffsquaresum_f64.c \
	mix_u8.c \
	multsum.c \
	scalaradd.c \
	scalarmult.c \
	sincos_f64.c \
	squaresum_f64.c \
	sum_f64.c

libsimdpack_la_SOURCES = \
	$(c_sources) $(i386_sources) $(powerpc_sources) $(amd64_sources)
libsimdpack_la_LIBADD = \
	$(opt_libs)
libsimdpack_la_CFLAGS = $(LIBOIL_CFLAGS)

noinst_HEADERS = \
	simdpack.h

libsimdpack_opt1_la_SOURCES = $(c_sources)
libsimdpack_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
	$(LIBOIL_OPT_CFLAGS)

