
if USE_ALT_OPT
opt_libs = libref_opt1.la
else
opt_libs =
endif

noinst_LTLIBRARIES = libref.la $(opt_libs)

c_sources = \
	argb_paint.c \
	ayuv2argb.c \
	composite.c \
	conv.c \
	copy.c \
	copy8x8.c \
	diff8x8.c \
	error8x8.c \
	permute.c \
	recon8x8.c \
	resample.c \
	rgb.c \
	rowcolsad8x8.c \
	sad8x8avg.c \
	splat.c \
	tablelookup.c \
	trans8x8.c \
	yuv.c

libref_la_SOURCES = \
	$(c_sources)
libref_la_LIBADD = \
	$(opt_libs)
libref_la_CFLAGS = $(LIBOIL_CFLAGS)

libref_opt1_la_SOURCES = $(c_sources)
libref_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) \
	$(LIBOIL_OPT_CFLAGS)

