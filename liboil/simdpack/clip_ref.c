/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>

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
OIL_DEFINE_CLASS(clip_ ## type, \
    "type_" #type " *dest, " \
    "int dstr, " \
    "type_" #type " *src, " \
    "int sstr, int n, " \
    "type_" #type " *s2_1, type_" #type " *s3_1"); \
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


