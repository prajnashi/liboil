
if USE_ALT_OPT
opt_libs = libconv_opt.la
else
opt_libs =
endif
noinst_LTLIBRARIES = libconv.la $(opt_libs)

c_sources = \
	conv_ref.c \
	conv_c.c \
	conv_bitstuff.c \
	conv_misc.c

if HAVE_CPU_POWERPC
powerpc_sources = conv_powerpc.c
else
powerpc_sources = 
endif

if HAVE_CPU_I386
i386_sources = conv_sse.c conv_3dnow.c
else
i386_sources = 
endif

if HAVE_CPU_AMD64
amd64_sources = conv_sse.c conv_3dnow.c
else
amd64_sources = 
endif

libconv_opt_la_SOURCES = $(c_sources)
libconv_opt_la_CFLAGS = $(LIBOIL_CFLAGS) $(LIBOIL_OPT_CFLAGS)

noinst_HEADERS = conv.h

libconv_la_SOURCES = \
	$(c_sources) \
	$(powerpc_sources) \
	$(i386_sources) \
	$(amd64_sources)
libconv_la_CFLAGS = $(LIBOIL_CFLAGS)
libconv_la_LIBADD = $(opt_libs)

