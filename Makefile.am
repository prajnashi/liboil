
SUBDIRS = liboil testsuite examples doc

EXTRA_DIST = COPYING autogen.sh

pkgconfig_DATA = liboil-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR).pc: liboil.pc
	cp liboil.pc liboil-$(LIBOIL_MAJORMINOR).pc

CLEANFILES = liboil-$(LIBOIL_MAJORMINOR).pc

ACLOCAL_FLAGS = -I m4

