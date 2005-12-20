
if USE_ALT_OPT
opt_libs = libref_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libref.la $(opt_libs)

c_sources = \
	abs.c \
	argb_paint.c \
	average2_u8.c \
	ayuv2argb.c \
	clamp.c \
	clip_ref.c \
	composite.c \
	conv.c \
	copy.c \
	copy8x8.c \
	diff8x8.c \
	diffsquaresum_f64.c \
	error8x8.c \
	math.c \
	mix_u8.c \
	mult8x8_s16.c \
	multsum.c \
	permute.c \
	recon8x8.c \
	resample.c \
	rgb.c \
	rowcolsad8x8.c \
	sad8x8.c \
	sad8x8_broken.c \
	sad8x8avg.c \
	scalaradd.c \
	scalarmult.c \
	sincos_f64.c \
	splat.c \
	squaresum_f64.c \
	sum_f64.c \
	swab.c \
	tablelookup.c \
	trans8x8.c \
	vectoradd_f64.c \
	vectoradd_s.c \
	yuv.c

libref_la_SOURCES = \
	$(c_sources)
libref_la_LIBADD = \
	$(opt_libs)
libref_la_CFLAGS = $(LIBOIL_CFLAGS)

libref_opt1_la_SOURCES = $(c_sources)
libref_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
	$(LIBOIL_OPT_CFLAGS)

