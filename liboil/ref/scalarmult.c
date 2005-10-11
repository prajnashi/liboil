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

