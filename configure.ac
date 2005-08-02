AC_PREREQ([2.58])
AC_INIT(liboil,0.3.2.1)

AS_NANO(LIBOIL_CVS=no,LIBOIL_CVS=yes)

AC_CANONICAL_TARGET([])
AM_INIT_AUTOMAKE(1.6)
AM_MAINTAINER_MODE

LIBOIL_MAJORMINOR=0.3
AC_SUBST(LIBOIL_MAJORMINOR)

AM_CONFIG_HEADER(config.h)

dnl CURRENT, REVISION, AGE
dnl - library source changed -> increment REVISION
dnl - interfaces added/removed/changed -> increment CURRENT, REVISION = 0
dnl - interfaces added -> increment AGE
dnl - interfaces removed -> AGE = 0
LIBOIL_LIBVERSION="1:0:1"
AC_SUBST(LIBOIL_LIBVERSION)
AM_PROG_LIBTOOL
AM_PROG_AS

AC_CONFIG_SRCDIR([liboil/liboil.h])

ACLOCAL_AMFLAGS="-I m4 $ACLOCAL_AMFLAGS"
AC_SUBST(ACLOCAL_AMFLAGS)

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

AC_ARG_ENABLE(prototype-checking,
  AC_HELP_STRING([--enable-prototype-checking],[compile with strict prototype checking]),
  enable_proto_check=$enableval,enable_proto_check=no)
if test "x$enable_proto_check" = xyes ; then
  AC_DEFINE(LIBOIL_STRICT_PROTOTYPES,1,[Defined if prototype checking enabled])
fi

AC_ARG_ENABLE(alternate-optimization,
  AC_HELP_STRING([--enable-alternate-optimization],[compile with alternate optimization flags]),
  enable_alt_opt=$enableval,enable_alt_opt=no)
AM_CONDITIONAL(USE_ALT_OPT, test "x$enable_alt_opt" = xyes)
LIBOIL_OPT_CFLAGS="-O3 -fomit-frame-pointer -funroll-all-loops -Wno-unused-function -D'OIL_OPT_MANGLE(a)=a\#\#_O3' -D'OIL_OPT_SUFFIX=\"_O3\"'"
AC_SUBST(LIBOIL_OPT_CFLAGS)

AC_ARG_ENABLE(broken-implementations,
  AC_HELP_STRING([--enable-broken-implementations],[compile with broken implementations]),
  enable_broken_implementations=$enableval,enable_broken_implementations=no)
if test "x$enable_broken_implementations" = xyes ; then
  AC_DEFINE(ENABLE_BROKEN_IMPLS, 1, [Define if compiling broken implementations])
fi

##################################################
# Check for gtk-doc.
##################################################

AC_ARG_WITH(html-dir, [  --with-html-dir=PATH path to installed docs ])

if test "x$with_html_dir" = "x" ; then
  HTML_DIR='${datadir}/gtk-doc/html'
else
  HTML_DIR=$with_html_dir
fi

AC_SUBST(HTML_DIR)

gtk_doc_min_version=1.0
AC_MSG_CHECKING([gtk-doc version >= $gtk_doc_min_version])
if pkg-config --atleast-version=$gtk_doc_min_version gtk-doc; then
  AC_MSG_RESULT(yes)
  GTKDOC=true
else
  AC_MSG_RESULT(no)
  GTKDOC=false
fi

dnl Let people disable the gtk-doc stuff.
AC_ARG_ENABLE(gtk-doc, [  --enable-gtk-doc  Use gtk-doc to build documentation [default=auto]], enable_gtk_doc="$enableval", enable_gtk_doc=auto)

if test x$enable_gtk_doc = xauto ; then
  if test x$GTKDOC = xtrue ; then
    enable_gtk_doc=yes
  else
    enable_gtk_doc=no 
  fi
fi

AM_CONDITIONAL(ENABLE_GTK_DOC, test x$enable_gtk_doc = xyes)

GTK_DOC_CHECK(1.0)


dnl Set up conditionals for (target) architecture:
dnl ==============================================

dnl Determine CPU
case "x${target_cpu}" in
  xi?86 | k?) HAVE_CPU_I386=yes
              AC_DEFINE(HAVE_CPU_I386, 1, [Define if the target CPU is an x86]) ;;
  xx86_64)    HAVE_CPU_AMD64=yes
              AC_DEFINE(HAVE_CPU_AMD64, 1, [Define if the target CPU is an amd64]) ;;
  xpowerpc)   HAVE_CPU_POWERPC=yes
              AC_DEFINE(HAVE_CPU_POWERPC, 1, [Define if the target CPU is a powerpc]) ;;
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
  xia64*)     HAVE_CPU_IA64=yes
              AC_DEFINE(HAVE_CPU_IA64, 1, [Define if the target CPU is a IA64]) ;;
esac

AM_CONDITIONAL(HAVE_CPU_I386, test "x$HAVE_CPU_I386" = "xyes")
AM_CONDITIONAL(HAVE_CPU_AMD64, test "x$HAVE_CPU_AMD64" = "xyes")
AM_CONDITIONAL(HAVE_CPU_POWERPC, test "x$HAVE_CPU_POWERPC" = "xyes")
AM_CONDITIONAL(HAVE_CPU_ALPHA, test "x$HAVE_CPU_ALPHA" = "xyes")
AM_CONDITIONAL(HAVE_CPU_ARM, test "x$HAVE_CPU_ARM" = "xyes")
AM_CONDITIONAL(HAVE_CPU_SPARC, test "x$HAVE_CPU_SPARC" = "xyes")
AM_CONDITIONAL(HAVE_CPU_MIPS, test "x$HAVE_CPU_MIPS" = "xyes")
AM_CONDITIONAL(HAVE_CPU_HPPA, test "x$HAVE_CPU_HPPA" = "xyes")
AM_CONDITIONAL(HAVE_CPU_IA64, test "x$HAVE_CPU_IA64" = "xyes")

AC_C_BIGENDIAN

AC_LTDL_SYMBOL_USCORE
if test x${ac_cv_sys_symbol_underscore} = xyes ; then
  AC_DEFINE(HAVE_SYMBOL_UNDERSCORE, 1, [Define if C symbols have leading underscore])
fi



AC_FUNC_MMAP()
AC_CHECK_LIB(m, rintf,
  AC_DEFINE(HAVE_RINTF, 1, [Define if rintf() is available]))
AC_CHECK_LIB(m, lrint,
  AC_DEFINE(HAVE_LRINT, 1, [Define if lrint() is available]))
AC_CHECK_LIB(m, lrintf,
  AC_DEFINE(HAVE_LRINTF, 1, [Define if lrintf() is available]))

AC_CHECK_HEADER(ieee754.h,
  AC_DEFINE(HAVE_IEEE754_H, 1, [Define if ieee754.h exists]))

AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([sigaction])

LIBM="-lm"
AC_SUBST(LIBM)

AS_COMPILER_FLAG(-Wall, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wall")
if test "x$LIBOIL_CVS" = "xyes"
then
  AS_COMPILER_FLAG(-Werror, LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Werror")
fi
if test x$HAVE_CPU_POWERPC = xyes ; then
  AS_COMPILER_FLAG(["-Wa,-mregnames"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-mregnames"],
    true)
  AS_COMPILER_FLAG(["-Wa,-maltivec"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -Wa,-maltivec"],
    true)
  AS_COMPILER_FLAG(["-force_cpusubtype_ALL"],
    [LIBOIL_CFLAGS="$LIBOIL_CFLAGS -force_cpusubtype_ALL"],
    true)
fi

if test x$HAVE_CPU_I386 = xyes -o x$HAVE_CPU_AMD64 = xyes ; then
  AS_COMPILER_FLAG(["-mmmx"], [MMX_CFLAGS="-mmmx"], true)
  AS_COMPILER_FLAG(["-msse"], [SSE_CFLAGS="-msse"], true)
  AS_COMPILER_FLAG(["-msse2"], [SSE2_CFLAGS="-msse2"], true)
  AS_COMPILER_FLAG(["-msse3"], [SSE3_CFLAGS="-msse3"], true)
  AS_COMPILER_FLAG(["-m3dnow"], [_3DNOW_CFLAGS="-m3dnow"], true)
fi
AC_SUBST(MMX_CFLAGS)
AC_SUBST(SSE_CFLAGS)
AC_SUBST(SSE2_CFLAGS)
AC_SUBST(SSE3_CFLAGS)
AC_SUBST(_3DNOW_CFLAGS)

LIBOIL_CFLAGS="$LIBOIL_CFLAGS -D_BSD_SOURCE -I\$(top_srcdir) -O2"
AC_SUBST(LIBOIL_CFLAGS)

LIBOIL_LIBS="\$(top_builddir)/liboil/liboil-$LIBOIL_MAJORMINOR.la"
AC_SUBST(LIBOIL_LIBS)

pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

#CFLAGS=`echo "$CFLAGS" | sed -e 's/-O[0-9*]//g'`
#CFLAGS="-g"

AC_CONFIG_FILES([
Makefile 
doc/Makefile
liboil/Makefile
liboil/colorspace/Makefile
liboil/conv/Makefile
liboil/copy/Makefile
liboil/dct/Makefile
liboil/fb/Makefile
liboil/math/Makefile
liboil/md5/Makefile
liboil/motovec/Makefile
liboil/jpeg/Makefile
liboil/simdpack/Makefile
liboil/sse/Makefile
liboil/utf8/Makefile
testsuite/Makefile
testsuite/instruction/Makefile
examples/Makefile
examples/huffman/Makefile
examples/jpeg/Makefile
examples/md5/Makefile
examples/taylor/Makefile
examples/uberopt/Makefile
examples/work/Makefile
liboil.pc
])
AC_OUTPUT



