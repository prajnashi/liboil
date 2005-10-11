/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>

#define CLIP_DEFINE_REF(type) \
static void clip_ ## type ## _ref ( \
    type_ ## type *dest, \
    int dstr, \
    type_ ## type *src, \
    int sstr, int n, \
    type_ ## type *min, type_ ## type *max) \
{ \
  int i; \
  type_ ## type x; \
  for(i=0;i<n;i++){ \
    x = OIL_GET(src,i*sstr,type_ ## type); \
    if (x<*min) x = *min; \
    if (x>*max) x = *max; \
    OIL_GET(dest,i*dstr,type_ ## type) = x; \
  } \
} \
static void clip_ ## type ## _test (OilTest *test) \
{ \
  type_ ## type *lo = (type_ ## type *) \
    (test->params[OIL_ARG_SRC2].src_data + OIL_TEST_HEADER); \
  type_ ## type *hi = (type_ ## type *) \
    (test->params[OIL_ARG_SRC3].src_data + OIL_TEST_HEADER); \
  if (*lo > *hi) { \
    type_ ## type tmp; \
    tmp = *lo; \
    *lo = *hi; \
    *hi = tmp; \
  } \
} \
OIL_DEFINE_CLASS_FULL(clip_ ## type, \
    "type_" #type " *dest, " \
    "int dstr, " \
    "type_" #type " *src, " \
    "int sstr, int n, " \
    "type_" #type " *s2_1, type_" #type " *s3_1", \
    clip_ ## type ## _test); \
OIL_DEFINE_IMPL_REF(clip_ ## type ## _ref, clip_ ## type)

CLIP_DEFINE_REF (s8);
CLIP_DEFINE_REF (u8);
CLIP_DEFINE_REF (s16);
CLIP_DEFINE_REF (u16);
CLIP_DEFINE_REF (s32);
CLIP_DEFINE_REF (u32);
CLIP_DEFINE_REF (f32);
CLIP_DEFINE_REF (f64);



#if 0
#ifdef HAVE_CPU_POWERPC
static void
clip_f64_ppcasm(f64 *dest, f64 *src, f64 low, f64 hi, int n)
{
	f64 ftmp;

	dest--;
	src--;
	__asm__ __volatile__("\n"
		"1:	lfdu 0,8(%1)\n"
		"	addic. %2,%2,-1\n"
		"	fsub 1,0,%3\n"
		"	fsel 0,1,0,%3\n"
		"	fsub 1,0,%4\n"
		"	fsel 0,1,%4,0\n"
		"	stfdu 0,8(%0)\n"
		"	bge 1b\n"
	: "+b" (dest), "+b" (src), "+r" (n)
	: "f" (low), "f" (hi), "b" (&ftmp)
	: "32", "33", "11" );
}
#endif
#endif


