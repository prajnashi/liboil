
if USE_ALT_OPT
opt_libs = libsimdpack_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libsimdpack.la $(opt_libs)

c_sources = \
	abs.c \
	abs_misc.c \
	average2_u8.c \
	clip_ref.c \
	clip_fast.c \
	diffsquaresum_f64.c \
	mix_u8.c \
	mult8x8_s16.c \
	multsum.c \
	sad8x8.c \
	sad8x8_broken.c \
	scalaradd.c \
	scalarmult.c \
	sincos_f64.c \
	squaresum_f64.c \
	sum_f64.c \
	vectoradd_f64.c

if HAVE_CPU_I386
i386_sources = \
	abs_i386.c \
	mult8x8_i386.c \
	scalarmult_i386.c
else
i386_sources =
endif

if HAVE_CPU_AMD64
amd64_sources = \
	abs_i386.c \
	mult8x8_i386.c \
	scalarmult_i386.c
else
amd64_sources =
endif

if HAVE_CPU_POWERPC
powerpc_sources = \
	abs_powerpc.c \
	clip_powerpc.c \
	mix_powerpc.c \
	multsum_powerpc.c \
	sad8x8_powerpc.c
else
powerpc_sources =
endif

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

