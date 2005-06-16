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
#include <emmintrin.h>

#include <../conv/conv.h>


static void
conv_f32_s32_sse (float *dst, int dst_stride, int32_t * src, int src_stride,
    int n)
{
  int i;
  uint32_t tmp_dest_array[68], *tmp_dest;
  int32_t tmp_src_array[68], *tmp_src;
  __m128 xmm0 = { 0 };
  __m128 xmm1 = { 0 };

  tmp_dest = (void *)(((unsigned long)(tmp_dest_array) + 0xf)&~0xf);
  tmp_src = (void *)(((unsigned long)(tmp_src_array) + 0xf)&~0xf);
  while (n>64) {
    for (i=0;i<64;i+=2){
      tmp_src[i] = OIL_GET(src, src_stride * i, int32_t);
      tmp_src[i+1] = OIL_GET(src, src_stride * (i+1), int32_t);
    }
    for (i=0;i<8;i++){
      xmm0 = _mm_cvt_pi2ps (xmm0, *(__m64 *)(tmp_src + 8*i + 2));
      xmm0 = _mm_movelh_ps (xmm0, xmm0);
      xmm0 = _mm_cvt_pi2ps (xmm0, *(__m64 *)(tmp_src + 8*i));
      _mm_store_ps (((float *)tmp_dest) + i*8, xmm0);
      xmm1 = _mm_cvt_pi2ps (xmm1, *(__m64 *)(tmp_src + 8*i + 6));
      xmm1 = _mm_movelh_ps (xmm1, xmm1);
      xmm1 = _mm_cvt_pi2ps (xmm1, *(__m64 *)(tmp_src + 8*i + 4));
      _mm_store_ps (((float *)tmp_dest) + i*8 + 4, xmm1);
    }
    for (i=0;i<64;i+=2){
      OIL_GET(dst, dst_stride * i, int32_t) = tmp_dest[i];
      OIL_GET(dst, dst_stride * (i+1), int32_t) = tmp_dest[i+1];
    }
    n-=64;
    src = OIL_OFFSET (src, src_stride * 64);
    dst = OIL_OFFSET (dst, dst_stride * 64);
  }
  for (i=0;i<n;i++){
    tmp_src[i] = OIL_GET(src, src_stride * i, int32_t);
  }
  for (i=0;i<(n+3)/4;i++){
    xmm0 = _mm_cvt_pi2ps (xmm0, ((__m64 *)tmp_src)[2*i+1]);
    xmm0 = _mm_movelh_ps (xmm0, xmm0);
    xmm0 = _mm_cvt_pi2ps (xmm0, ((__m64 *)tmp_src)[2*i]);
    _mm_store_ps (((float *)tmp_dest) + i*4, xmm0);
  }
  for (i=0;i<n;i++){
    OIL_GET(dst, dst_stride * i, int32_t) = tmp_dest[i];
  }
}
OIL_DEFINE_IMPL_FULL (conv_f32_s32_sse, conv_f32_s32, OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2);

