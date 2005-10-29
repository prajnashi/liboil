dnl as-intrinsics.m4 0.1.0

dnl autostars m4 macro for detection of gcc inline assembly

dnl David Schleef <ds@schleef.org>

dnl $Id: as-intrinsics.m4,v 1.2 2005-10-29 02:32:33 ds Exp $

dnl AS_MMX_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_MMX_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports MMX intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -mmmx"
  AC_TRY_COMPILE([#include <mmintrin.h>], [_mm_empty()], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-mmmx"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_SSE_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_SSE_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports SSE intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -msse"
  AC_TRY_COMPILE([#include <xmmintrin.h>], [__m128 a; a = _mm_or_ps(a,a)], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-msse"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_SSE2_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_SSE2_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports SSE2 intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -msse2"
  AC_TRY_COMPILE([#include <emmintrin.h>], [__m128d a; a = _mm_setzero_pd()], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-msse2"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_SSE3_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_SSE3_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports SSE3 intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -msse3"
  AC_TRY_COMPILE([#include <pmmintrin.h>], [__m128 a; a = _mm_addsub_ps(a,a)], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-msse3"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_3DNOW_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_3DNOW_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports 3DNOW intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -m3dnow"
  AC_TRY_COMPILE([#include <mm3dnow.h>], [_m_femms()], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-m3dnow"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_3DNOWEXT_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_3DNOWEXT_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports 3DNOWEXT intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -m3dnow -march=athlon"
  AC_TRY_COMPILE([#include <mm3dnow.h>], [__m64 a;a=_m_pf2iw(a)], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-m3dnow -march=athlon"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])


dnl AS_ALTIVEC_INTRINSICS(CFLAGS, ACTION-IF-ACCEPTED, [ACTION-IF-NOT-ACCEPTED])
dnl Tries to compile with the given CFLAGS.
dnl Runs ACTION-IF-ACCEPTED if the compiler can compile with the flags,
dnl and ACTION-IF-NOT-ACCEPTED otherwise.

AC_DEFUN([AS_ALTIVEC_INTRINSICS],
[
  AC_MSG_CHECKING([if compiler supports ALTIVEC intrinsics])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -maltivec"
  AC_TRY_COMPILE([#include <altivec.h>], [__vector_float a; a = vec_or(a,a)], [flag_ok=yes], [flag_ok=no])
  CFLAGS="$save_CFLAGS"

  $1="-maltivec"
  if test "X$flag_ok" = Xyes ; then
    $2
    true
  else
    $3
    true
  fi
  AC_MSG_RESULT([$flag_ok])
])

