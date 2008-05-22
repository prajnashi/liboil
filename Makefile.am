
AUTOMAKE_OPTIONS = foreign

SUBDIRS = liboil orc testsuite examples doc

EXTRA_DIST = COPYING autogen.sh gtk-doc.make HACKING BUG-REPORTING

DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc

pkgconfig_DATA = liboil-$(LIBOIL_MAJORMINOR).pc orc-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR).pc: liboil.pc
	cp liboil.pc liboil-$(LIBOIL_MAJORMINOR).pc

liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc: liboil-uninstalled.pc
	cp liboil-uninstalled.pc liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

orc-$(LIBOIL_MAJORMINOR).pc: orc.pc
	cp orc.pc orc-$(LIBOIL_MAJORMINOR).pc

orc-$(LIBOIL_MAJORMINOR)-uninstalled.pc: orc-uninstalled.pc
	cp orc-uninstalled.pc orc-$(LIBOIL_MAJORMINOR)-uninstalled.pc

BUILT_SOURCES=liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

CLEANFILES = liboil-$(LIBOIL_MAJORMINOR).pc liboil-$(LIBOIL_MAJORMINOR)-uninstalled.pc

ACLOCAL_FLAGS = -I m4

