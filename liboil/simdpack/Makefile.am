
if USE_ALT_OPT
opt_libs = libsimdpack_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libsimdpack.la $(opt_libs)

libsimdpack_la_SOURCES = \
	abs.c \
	abs_u16_s16.c \
	abs_u32_s32.c \
	average2_u8.c \
	clip_ref.c \
	clip_s16.c \
	clip_s32.c \
	diffsquaresum_f64.c \
	mix_u8.c \
	mult8x8_s16.c \
	multsum.c \
	sad8x8.c \
	scalaradd.c \
	scalarmult.c \
	sincos_f64.c \
	squaresum_f64.c \
	sum_f64.c \
	vectoradd_f64.c
libsimdpack_la_LIBADD = \
	$(opt_libs)

libsimdpack_opt1_la_SOURCES = $(libsimdpack_la_SOURCES)

noinst_HEADERS = \
	simdpack.h

libsimdpack_la_CFLAGS = $(LIBOIL_CFLAGS)

libsimdpack_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
	$(LIBOIL_OPT_CFLAGS)

