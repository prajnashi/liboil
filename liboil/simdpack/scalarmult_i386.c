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

static void
scalarmult_f32_sse (float *dest, int dstr, float *src, int sstr,
    float *val, int n)
{
  float tmp[8];
  float *t = (void *)(((unsigned long)tmp & ~0xf) + 16);
  int i;

  t[0] = *val;
  t[1] = *val;
  t[2] = *val;
  t[3] = *val;
  asm volatile (
      "  movss (%0), %%xmm1 \n"
      : 
      : "r" (t));
  for(i=0;i<n-3;i+=4) {
    t[0] = OIL_GET(src,sstr*(i + 0), float);
    t[1] = OIL_GET(src,sstr*(i + 1), float);
    t[2] = OIL_GET(src,sstr*(i + 2), float);
    t[3] = OIL_GET(src,sstr*(i + 3), float);
    asm volatile (
        "  movss (%0), %%xmm0 \n"
        "  mulps %%xmm1, %%xmm0 \n"
        "  movss %%xmm0, (%0) \n"
        : 
        : "r" (t));
    OIL_GET(dest,dstr*(i + 0), float) = t[0];
    OIL_GET(dest,dstr*(i + 1), float) = t[1];
    OIL_GET(dest,dstr*(i + 2), float) = t[2];
    OIL_GET(dest,dstr*(i + 3), float) = t[3];
  }
  for(;i<n;i++){
    OIL_GET(dest,dstr*i, float) = *val * OIL_GET(src,sstr*i, float);
  }
}
OIL_DEFINE_IMPL_FULL (scalarmult_f32_sse, scalarmult_f32, OIL_IMPL_FLAG_SSE);


