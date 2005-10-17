
noinst_LTLIBRARIES = libpowerpc.la

if HAVE_ASM_BLOCKS
asm_block_sources = \
	recon8x8_ppc.c  \
	recon8x8_altivec.c  
else
asm_block_sources =
endif

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
libpowerpc_la_SOURCES = \
	$(asm_block_sources)
	$(sources)
libpowerpc_la_CFLAGS = $(LIBOIL_CFLAGS) -maltivec -mabi=altivec -fasm-blocks
else
libpowerpc_la_SOURCES = \
	empty.c
endif

empty.c:
	echo >empty.c
