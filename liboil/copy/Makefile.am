
if USE_ALT_OPT
opt_libs = libcopy_opt.la
else
opt_libs = 
endif

noinst_LTLIBRARIES = libcopy.la $(opt_libs)

if HAVE_CPU_POWERPC
powerpc_sources = \
	copy_powerpc.c
else
powerpc_sources =
endif

if HAVE_CPU_I386
i386_sources = \
	copy_i386.c \
	trans8x8_i386.c
else
i386_sources =
endif

c_sources = \
	copy.c \
	permute.c \
	splat_ref.c \
	tablelookup_ref.c \
	trans8x8.c

libcopy_la_SOURCES = \
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources)
libcopy_la_LIBADD = $(opt_libs)

libcopy_la_CFLAGS = $(LIBOIL_CFLAGS)

libcopy_opt_la_SOURCES = $(c_sources)
libcopy_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)


