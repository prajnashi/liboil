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
#include <liboil/simdpack/simdpack.h>
#include <liboil/liboilgcc.h>
#include <string.h>

#define SCALARADD_DEFINE_REF(type)		\
static void scalaradd_ ## type ## _ref(		\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  int i;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, type_ ## type) =       \
      OIL_GET(src,sstr*i, type_ ## type) + *val; \
  }						\
}						\
OIL_DEFINE_CLASS (scalaradd_ ## type,         \
    "type_" #type " *dest, int dstr, "		\
    "type_" #type " *src, int sstr, "		\
    "type_" #type " *s2_1 int n");		\
OIL_DEFINE_IMPL_REF (scalaradd_ ## type ## _ref, scalaradd_ ## type);


SCALARADD_DEFINE_REF (s8);
SCALARADD_DEFINE_REF (u8);
SCALARADD_DEFINE_REF (s16);
SCALARADD_DEFINE_REF (u16);
SCALARADD_DEFINE_REF (s32);
SCALARADD_DEFINE_REF (u32);
SCALARADD_DEFINE_REF (f32);
SCALARADD_DEFINE_REF (f64);

#define SCALARADD_DEFINE_UNROLL2(type)		\
static void scalaradd_ ## type ## _unroll2(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 2;					\
  while(n>0){					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll2, scalaradd_ ## type);

SCALARADD_DEFINE_UNROLL2 (s8);
SCALARADD_DEFINE_UNROLL2 (u8);
SCALARADD_DEFINE_UNROLL2 (s16);
SCALARADD_DEFINE_UNROLL2 (u16);
SCALARADD_DEFINE_UNROLL2 (s32);
SCALARADD_DEFINE_UNROLL2 (u32);
SCALARADD_DEFINE_UNROLL2 (f32);
SCALARADD_DEFINE_UNROLL2 (f64);

#define SCALARADD_DEFINE_UNROLL2x(type)		\
static void scalaradd_ ## type ## _unroll2x(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  type_ ## type *dest2;				\
  type_ ## type *src2;				\
  int i;					\
  if(n&1) {					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  src2 = OIL_OFFSET(src, sstr);			\
  dest2 = OIL_OFFSET(dest, dstr);		\
  n /= 2;					\
  sstr *= 2;					\
  dstr *= 2;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, type_ ## type) = OIL_GET(src,sstr*i, type_ ## type) + *val; \
    OIL_GET(dest2,dstr*i, type_ ## type) = OIL_GET(src2,sstr*i, type_ ## type) + *val; \
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll2x, scalaradd_ ## type);

SCALARADD_DEFINE_UNROLL2x (s8);
SCALARADD_DEFINE_UNROLL2x (u8);
SCALARADD_DEFINE_UNROLL2x (s16);
SCALARADD_DEFINE_UNROLL2x (u16);
SCALARADD_DEFINE_UNROLL2x (s32);
SCALARADD_DEFINE_UNROLL2x (u32);
SCALARADD_DEFINE_UNROLL2x (f32);
SCALARADD_DEFINE_UNROLL2x (f64);

#define SCALARADD_DEFINE_UNROLL4(type)		\
static void scalaradd_ ## type ## _unroll4(	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src, int sstr,		\
    type_ ## type *val, int n)			\
{						\
  if(n&1) {					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  if(n&2) {					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			\
  }						\
  n /= 4;					\
  while(n>0){					\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    *dest = *src + *val;			\
    OIL_INCREMENT(dest,dstr);			\
    OIL_INCREMENT(src,sstr);			\
    n--;					\
  }						\
}						\
OIL_DEFINE_IMPL (scalaradd_ ## type ## _unroll4, scalaradd_ ## type);

SCALARADD_DEFINE_UNROLL4 (s8);
SCALARADD_DEFINE_UNROLL4 (u8);
SCALARADD_DEFINE_UNROLL4 (s16);
SCALARADD_DEFINE_UNROLL4 (u16);
SCALARADD_DEFINE_UNROLL4 (s32);
SCALARADD_DEFINE_UNROLL4 (u32);
SCALARADD_DEFINE_UNROLL4 (f32);
SCALARADD_DEFINE_UNROLL4 (f64);


static void scalaradd_s8_mmx(	
    type_s8 *dest, int dstr,		
    type_s8 *src, int sstr,		
    type_s8 *val, int n)			
{
  vec_s8 vval;
  vec_s8 *vsrc;
  vec_s8 *vdest;
//  int i;
  vsrc = (vec_s8*)src;
  vdest = (vec_s8*)dest;
  memset(&vval,*val,8);						
//  for (i=0;i<8;i++) vval[i]=*val;
  if(n&1) {					
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			\
    OIL_INCREMENT(src, sstr);			
  }						  
  if(n&2) {					\
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
  }						
  if(n&4) {					\
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
    *dest = *src + *val;			
    OIL_INCREMENT(dest, dstr);			
    OIL_INCREMENT(src, sstr);			
  }						
  n /= 8;					  
  
  while(n>0){					
    *vdest = *vsrc + vval;			
    OIL_INCREMENT(vdest,8);			
    OIL_INCREMENT(vsrc,8);			
    n--;					
  }						
}						
OIL_DEFINE_IMPL_FULL(scalaradd_s8_mmx,scalaradd_s8,OIL_IMPL_FLAG_MMX);
