
noinst_LTLIBRARIES = libpowerpc.la

sources = \
	abs.c \
	clip.c \
	conv.c \
	copy.c \
	fdct8x8theora_altivec.c \
	md5.c \
	mix.c \
	multsum.c \
	recon8x8_altivec.c \
	recon8x8_ppc.c \
	resample.c \
	rgb2bgr.c \
	rgb2rgba.c \
	sad8x8.c \
	splat.c \
	zigzag8x8.c

if HAVE_CPU_POWERPC
powerpc_sources = $(sources)
else
powerpc_sources =
endif

libpowerpc_la_SOURCES = \
	$(powerpc_sources)
libpowerpc_la_CFLAGS = $(LIBOIL_CFLAGS) -maltivec -mabi=altivec -fasm-blocks

