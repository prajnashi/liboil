
CLEANFILES = empty.c

noinst_LTLIBRARIES = libi386.la

sources = \
	abs_i386.c \
	argb_paint_i386.c \
	ayuv2argb_i386.c \
	composite_i386.c \
	copy8x8_i386.c \
	copy_i386.c \
	diff8x8_i386.c \
	error8x8_i386.c \
	fdct8x8theora_i386.c \
	idct8x8_i386.c \
	md5_i386.c \
	mult8x8_i386.c \
	recon8x8_i386.c \
	rowcolsad8x8_i386.c \
	sad8x8_i386.c \
	sad8x8avg_i386.c \
	scalarmult_i386.c \
	splat_i386.c \
	trans8x8_i386.c

if HAVE_GCC_I386
i386_sources = $(sources)
else
i386_sources = empty.c
endif

if HAVE_GCC_AMD64
amd64_sources = $(sources)
else
amd64_sources = empty.c
endif

libi386_la_SOURCES = \
	$(i386_sources)
libi386_la_CFLAGS = $(LIBOIL_CFLAGS)

empty.c:
	echo >empty.c

