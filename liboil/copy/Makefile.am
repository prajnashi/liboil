
if USE_ALT_OPT
opt_libs = libcopy_opt.la
else
opt_libs = 
endif

noinst_LTLIBRARIES = libcopy.la $(opt_libs)

libcopy_la_SOURCES = \
	copy.c \
	permute.c \
	splat_ref.c \
	tablelookup_ref.c \
	trans8x8.c
libcopy_la_LIBADD = $(opt_libs)

#	trans8x8_f32.c
#	trans8x8_s16.c

libcopy_la_CFLAGS = $(LIBOIL_CFLAGS)

libcopy_opt_la_SOURCES = $(libcopy_la_SOURCES)
libcopy_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)


