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
#include <liboil/simdpack/simdpack.h>

#define SCALARMULT_DEFINE_REF(type)		\
static void scalarmult_ ## type ## _ref(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  int i;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i,type_ ## type) = OIL_GET(src,sstr*i,type_ ## type) * *val; \
  }						\
}						\
OIL_DEFINE_CLASS(scalarmult_ ## type,           \
    "type_" #type " *dest, int dstr, "          \
    "type_" #type " *src, int sstr, "           \
    "type_" #type " *s2_1, int n");            \
OIL_DEFINE_IMPL_REF (scalarmult_ ## type ## _ref, scalarmult_ ## type);


SCALARMULT_DEFINE_REF (s8);
SCALARMULT_DEFINE_REF (u8);
SCALARMULT_DEFINE_REF (s16);
SCALARMULT_DEFINE_REF (u16);
SCALARMULT_DEFINE_REF (s32);
SCALARMULT_DEFINE_REF (u32);
SCALARMULT_DEFINE_REF (f32);
SCALARMULT_DEFINE_REF (f64);

#define SCALARMULT_DEFINE_UNROLL2(type)		\
static void scalarmult_ ## type ## _unroll2(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 2;					\
  while(n>0){					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalarmult_ ## type ## _unroll2, scalarmult_ ## type);

SCALARMULT_DEFINE_UNROLL2 (s8);
SCALARMULT_DEFINE_UNROLL2 (u8);
SCALARMULT_DEFINE_UNROLL2 (s16);
SCALARMULT_DEFINE_UNROLL2 (u16);
SCALARMULT_DEFINE_UNROLL2 (s32);
SCALARMULT_DEFINE_UNROLL2 (u32);
SCALARMULT_DEFINE_UNROLL2 (f32);
SCALARMULT_DEFINE_UNROLL2 (f64);

#define SCALARMULT_DEFINE_UNROLL2x(type)	\
static void scalarmult_ ## type ## _unroll2x(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  type_ ## type *dest2;				\
  type_ ## type *src2;				\
  int i;					\
  if(n&1) {					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  src2 = OIL_OFFSET(src, sstr);			\
  dest2 = OIL_OFFSET(dest, dstr);		\
  n /= 2;					\
  sstr *= 2;					\
  dstr *= 2;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i,type_ ## type) = OIL_GET(src,sstr*i,type_ ## type) * *val; \
    OIL_GET(dest2,dstr*i,type_ ## type) = OIL_GET(src2,sstr*i,type_ ## type) * *val; \
  }						\
}						\
OIL_DEFINE_IMPL (scalarmult_ ## type ## _unroll2x, scalarmult_ ## type);

SCALARMULT_DEFINE_UNROLL2x (s8);
SCALARMULT_DEFINE_UNROLL2x (u8);
SCALARMULT_DEFINE_UNROLL2x (s16);
SCALARMULT_DEFINE_UNROLL2x (u16);
SCALARMULT_DEFINE_UNROLL2x (s32);
SCALARMULT_DEFINE_UNROLL2x (u32);
SCALARMULT_DEFINE_UNROLL2x (f32);
SCALARMULT_DEFINE_UNROLL2x (f64);

#define SCALARMULT_DEFINE_UNROLL4(type)		\
static void scalarmult_ ## type ## _unroll4(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  if(n&2) {					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 4;					\
  while(n>0){					\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src * *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalarmult_ ## type ## _unroll4, scalarmult_ ## type);

SCALARMULT_DEFINE_UNROLL4 (s8);
SCALARMULT_DEFINE_UNROLL4 (u8);
SCALARMULT_DEFINE_UNROLL4 (s16);
SCALARMULT_DEFINE_UNROLL4 (u16);
SCALARMULT_DEFINE_UNROLL4 (s32);
SCALARMULT_DEFINE_UNROLL4 (u32);
SCALARMULT_DEFINE_UNROLL4 (f32);
SCALARMULT_DEFINE_UNROLL4 (f64);



