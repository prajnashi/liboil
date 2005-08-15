
noinst_LTLIBRARIES = libpowerpc.la

sources = \
	recon8x8_ppc.c  \
	recon8x8_altivec.c  

if HAVE_CPU_POWERPC
powerpc_sources = $(sources)
else
powerpc_sources =
endif

libpowerpc_la_SOURCES = \
	$(powerpc_sources)
libpowerpc_la_CFLAGS = $(LIBOIL_CFLAGS) -maltivec -mabi=altivec -fasm-blocks

