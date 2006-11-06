
AUTOMAKE_OPTIONS = foreign

SUBDIRS = liboil testsuite examples doc

EXTRA_DIST = COPYING autogen.sh gtk-doc.make HACKING BUG-REPORTING

pkgconfig_DATA = liboil-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR).pc: liboil.pc
	cp liboil.pc liboil-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc: liboil-uninstalled.pc
	cp liboil-uninstalled.pc liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

BUILT_SOURCES=liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

CLEANFILES = liboil-$(LIBOIL_MAJORMINOR).pc liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

ACLOCAL_FLAGS = -I m4

