/*
 * Copyright (c) 2005
 *	Eric Anholt.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilclasses.h>
#include <liboil/liboilfunction.h>
#include <emmintrin.h>

static void
composite_add_u8_const_src_sse (uint8_t *dest, uint8_t *src1_1, int n)
{
  __m128i xmm1;
  uint8_t val = *src1_1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    int x = *dest + val;
    if (x > 255) 
      x = 255;
    *dest++ = x;
  }
  xmm1 = _mm_set1_epi8(val);
  for (; n >= 16; n -= 16) {
    __m128i xmm0;
    xmm0 = _mm_load_si128((__m128i *)dest);
    xmm0 = _mm_adds_epu8(xmm0, xmm1);
    _mm_store_si128((__m128i *)dest, xmm0);
    dest += 16;
  }
  for (; n > 0; n--) {
    int x = *dest + val;
    if (x > 255) 
      x = 255;
    *dest++ = x;
  }
}
OIL_DEFINE_IMPL_FULL (composite_add_u8_const_src_sse,
    composite_add_u8_const_src, OIL_IMPL_FLAG_SSE2);
