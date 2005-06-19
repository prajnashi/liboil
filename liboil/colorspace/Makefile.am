
if USE_ALT_OPT
opt_libs = libcolorspace_opt1.la
else
opt_libs = 
endif
noinst_LTLIBRARIES = libcolorspace.la $(opt_libs)

c_sources = \
	rgb2bgr.c 		\
	rgb2rgba.c \
	ayuv2argb.c \
	argb_paint.c \
	composite.c \
	resample.c \
	yuv.c

if HAVE_CPU_POWERPC
powerpc_sources = \
	rgb2bgr_powerpc.c \
	rgb2rgba_powerpc.c \
	resample_powerpc.c
else
powerpc_sources =
endif

if HAVE_CPU_I386
i386_sources = \
	ayuv2argb_i386.c \
	argb_paint_i386.c
else
i386_sources =
endif

libcolorspace_la_SOURCES = 	\
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources)
libcolorspace_la_LIBADD = $(opt_libs)
libcolorspace_la_CFLAGS = $(LIBOIL_CFLAGS)

libcolorspace_opt1_la_SOURCES = $(c_sources)
libcolorspace_opt1_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

