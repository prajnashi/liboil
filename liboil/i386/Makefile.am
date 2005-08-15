
CLEANFILES = empty.c

noinst_LTLIBRARIES = libi386.la

sources = \
	error8x8_i386.c \
	recon8x8_i386.c \
	rowcolsad8x8_i386.c \
	sad8x8_i386.c \
	sad8x8avg_i386.c \
	diff8x8_i386.c

if HAVE_CPU_I386
i386_sources = $(sources)
else
i386_sources = empty.c
endif

if HAVE_CPU_AMD64
amd64_sources = $(sources)
else
amd64_sources = empty.c
endif

libi386_la_SOURCES = \
	$(i386_sources)
libi386_la_CFLAGS = $(LIBOIL_CFLAGS)

empty.c:
	echo >empty.c

