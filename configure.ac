AC_INIT
AC_CANONICAL_TARGET([])

AS_VERSION(liboil, LIBOIL_VERSION, 0, 2, 2, 1, LIBOIL_CVS="no", LIBOIL_CVS="yes")

AM_MAINTAINER_MODE
AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

#LIBOIL_MAJORMINOR=$LIBOIL_VERSION_MAJOR.$LIBOIL_VERSION_MINOR
LIBOIL_MAJORMINOR=0.3
AC_SUBST(LIBOIL_MAJORMINOR)

AM_CONFIG_HEADER(config.h)

dnl CURRENT, REVISION, AGE
dnl - library source changed -> increment REVISION
dnl - interfaces added/removed/changed -> increment CURRENT, REVISION = 0
dnl - interfaces added -> increment AGE
dnl - interfaces removed -> AGE = 0
AS_LIBTOOL(LIBOIL, 0, 0, 0)

AC_CONFIG_SRCDIR([liboil/liboil.h])

ACLOCAL="$ACLOCAL -I m4 $ACLOCAL_FLAGS"

AC_PROG_CC([c99])
#AM_PROG_AS

PKG_CHECK_MODULES(GLIB, glib-2.0, HAVE_GLIB=yes, HAVE_GLIB=no)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_ARG_ENABLE(glib,
AC_HELP_STRING([--disable-glib],[disable usage of glib]),
[case "${enableval}" in
  yes) HAVE_GLIB=yes ;;
  no) HAVE_GLIB=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-glib) ;;
esac])
AM_CONDITIONAL(HAVE_GLIB, test "x$HAVE_GLIB" = "xyes")

dnl If we have GNU tools, we can use a few special tricks.
dnl Basically, this means Linux for now.
dnl Disable this for now
#if test "x${target_os}" = "xlinux" -o "x${target_os}" = "xlinux-gnu" ; then
if false ; then
  HAVE_GNU_LINKER=yes
  AC_DEFINE(HAVE_GNU_LINKER, 1, [Define if GNU linker is being used])
else
  HAVE_GNU_LINKER=no
fi
AM_CONDITIONAL(HAVE_GNU_LINKER, test "x${HAVE_GNU_LINKER}" = "xyes")

dnl Set up conditionals for (target) architecture:
dnl ==============================================

dnl Determine CPU
case "x${target_cpu}" in
  xi?86 | k?) HAVE_CPU_I386=yes
              AC_DEFINE(HAVE_CPU_I386, 1, [Define if the target CPU is an x86]) ;;
  xpowerpc)   HAVE_CPU_POWERPC=yes
              AC_DEFINE(HAVE_CPU_POWERPC, 1, [Define if the target CPU is a POWERPC]) ;;
  xalpha)     HAVE_CPU_ALPHA=yes
              AC_DEFINE(HAVE_CPU_ALPHA, 1, [Define if the target CPU is an Alpha]) ;;
  xarm*)      HAVE_CPU_ARM=yes
              AC_DEFINE(HAVE_CPU_ARM, 1, [Define if the target CPU is an ARM]) ;;
  xsparc*)    HAVE_CPU_SPARC=yes
              AC_DEFINE(HAVE_CPU_SPARC, 1, [Define if the target CPU is a Sparc]) ;;
  xmips*)     HAVE_CPU_MIPS=yes
              AC_DEFINE(HAVE_CPU_MIPS, 1, [Define if the target CPU is a MIPS]) ;;
  xhppa*)     HAVE_CPU_HPPA=yes
              AC_DEFINE(HAVE_CPU_HPPA, 1, [Define if the target CPU is a HPPA]) ;;
esac

AM_CONDITIONAL(HAVE_CPU_I386,	test "x$HAVE_CPU_I386" = "xyes")
AM_CONDITIONAL(HAVE_CPU_POWERPC,	test "x$HAVE_CPU_POWERPC" = "xyes")

AC_C_BIGENDIAN

AC_CHECK_LIB(m, rintf,
  AC_DEFINE(HAVE_RINTF, 1, [Define if rintf() is available]))
AC_CHECK_LIB(m, lrint,
  AC_DEFINE(HAVE_LRINT, 1, [Define if lrint() is available]))
AC_CHECK_LIB(m, lrintf,
  AC_DEFINE(HAVE_LRINTF, 1, [Define if lrintf() is available]))

AS_COMPILER_FLAG(-Wall, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wall")
if test "x$LIBOIL_CVS" = "xyes"
then
  AS_COMPILER_FLAG(-Werror, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Werror")
fi
if test x$HAVE_CPU_POWERPC = xyes ; then
  AS_COMPILER_FLAG(["-Wa,-mregnames"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-mregnames"],
    true)
fi
if test x$HAVE_CPU_POWERPC = xyes ; then
  AS_COMPILER_FLAG(["-Wa,-maltivec"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-maltivec"],
    true)
fi

LIBOIL_CFLAGS="$LIBOIL_CFLAGS -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -I\$(top_srcdir)"
AC_SUBST(LIBOIL_CFLAGS)

LIBOIL_LIBS="\$(top_builddir)/liboil/liboil-$LIBOIL_MAJORMINOR.la"
AC_SUBST(LIBOIL_LIBS)

pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

AC_OUTPUT(
Makefile 
liboil/Makefile
liboil/copy/Makefile
liboil/dct/Makefile
liboil/md5/Makefile
liboil/jpeg/Makefile
liboil/simdpack/Makefile
testsuite/Makefile
examples/Makefile
examples/jpeg/Makefile
examples/md5/Makefile
liboil.pc
)



