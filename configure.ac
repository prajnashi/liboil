AC_INIT
AC_CANONICAL_TARGET([])

AS_VERSION(liboil, LIBOIL_VERSION, 0, 1, 0, 1, LIBOIL_CVS="no", LIBOIL_CVS="yes")

AM_MAINTAINER_MODE
AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

LIBOIL_MAJORMINOR=$LIBOIL_VERSION_MAJOR.$LIBOIL_VERSION_MINOR
AC_SUBST(LIBOIL_MAJORMINOR)

AM_CONFIG_HEADER(config.h)

AS_LIBTOOL(LIBOIL, 0, 0, 0)

AC_CONFIG_SRCDIR([liboil/liboil.h])

ACLOCAL="$ACLOCAL -I m4 $ACLOCAL_FLAGS"

AC_PROG_CC([c99])
#AM_PROG_AS

PKG_CHECK_MODULES(GLIB, glib-2.0, HAVE_GLIB=yes, HAVE_GLIB=no)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GLIB_CFLAGS)
if test x"$HAVE_GLIB" = "xno"; then
  AC_MSG_ERROR([cannot find glib-2.0, which is required to build liboil])
fi


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

LIBOIL_CFLAGS="$LIBOIL_CFLAGS -D_POSIX_C_SOURCE=200112L -I\$(top_srcdir)"
AC_SUBST(LIBOIL_CFLAGS)

LIBOIL_LIBS="\$(top_builddir)/liboil/liboil-$LIBOIL_MAJORMINOR.la $GLIB_LIBS"
AC_SUBST(LIBOIL_LIBS)

pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

AC_OUTPUT(
Makefile 
liboil/Makefile
liboil/jpeg/Makefile
liboil/simdpack/Makefile
testsuite/Makefile
examples/Makefile
liboil-0.1.pc
liboil-0.1-uninstalled.pc
)



