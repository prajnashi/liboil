
SUBDIRS = liboil testsuite examples

EXTRA_DIST = COPYING

pkgconfig_DATA = liboil-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR).pc: liboil.pc
	cp liboil.pc liboil-$(LIBOIL_MAJORMINOR).pc

CLEANFILES = liboil-$(LIBOIL_MAJORMINOR).pc
