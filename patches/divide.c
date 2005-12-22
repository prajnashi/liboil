/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
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

#include <math.h>

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include <stdio.h>


static void
divide_u32_u64_u32_test (OilTest *test)
{
  int i;
  int n;
  uint64_t *src1;
  uint32_t *src2;

  src1 = (uint64_t *)oil_test_get_source_data(test, OIL_ARG_SRC1);
  src2 = (uint32_t *)oil_test_get_source_data(test, OIL_ARG_SRC2);
  n = p1->post_n;
  for(i=0;i<n;i++){
    src2[i] = oil_rand_u32();
    src1[i] = oil_rand_u32() * (uint64_t)src2[i];
  }
}

OIL_DEFINE_CLASS_FULL (divide_u32_u64_u32, "uint32_t *dest, uint64_t *src1, uint32_t *src2, int n",
    divide_u32_u64_u32_test);


static void
divide_u32_u64_u32_ref (uint32_t *dest, uint64_t *src1, uint32_t *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] / src2[i];
  }

}
OIL_DEFINE_IMPL_REF (divide_u32_u64_u32_ref, divide_u32_u64_u32);

static void
divide_u32_u64_u32_long (uint32_t *dest, uint64_t *src1, uint32_t *src2, int n)
{
  int i;
  int bit;
  uint64_t a;
  uint32_t b;
  uint32_t c;

  for(i=0;i<n;i++){
    a = src1[i];
    b = src2[i];
    c = 0;
    for (bit = 31; bit >= 0; bit--) {
      if (a >= ((uint64_t)b)<<bit) {
        c |= 1<<bit;
        a -= ((uint64_t)b)<<bit;
      }
    }
    dest[i] = c;
  }

}
OIL_DEFINE_IMPL (divide_u32_u64_u32_long, divide_u32_u64_u32);

static int
binlog(uint32_t x)
{
  int y = 0;
  if (x &0xffff0000) y += 16;
  if (x &0xff00ff00) y += 8;
  if (x &0xf0f0f0f0) y += 4;
  if (x &0xcccccccc) y += 2;
  if (x &0xaaaaaaaa) y += 1;
  return y;
}

static void
divide_u32_u64_u32_long2 (uint32_t *dest, uint64_t *src1, uint32_t *src2, int n)
{
  int i;
  int bit;
  uint64_t a;
  uint32_t b;
  uint32_t c;
  uint32_t d;
  int shift;

  for(i=0;i<n;i++){
    a = src1[i];
    b = src2[i];

    d = b;
    shift = binlog(b);

    d = a>>shift;
    c = (d/b) << shift;
    a -= (uint64_t)c * b;

    if (a > 0) {
      for (bit = 31; bit >= 0; bit--) {
        if (a >= ((uint64_t)b)<<bit) {
          c += 1<<bit;
          a -= ((uint64_t)b)<<bit;
        }
        if (a==0) break;
      }
    }
    dest[i] = c;
  }

}
OIL_DEFINE_IMPL (divide_u32_u64_u32_long2, divide_u32_u64_u32);

static void
divide_u32_u64_u32_asm (uint32_t *dest, uint64_t *src1, uint32_t *src2, int n)
{
  int i;
  uint32_t a;
  uint32_t d;

  for(i=0;i<n;i++){
    a = src1[i];
    d = (src1[i]>>32);

    __asm__ __volatile__ ("\n"
        "  div %2\n"
        : "+a" (a), "+d" (d)
        : "r" (src2[i]));

    dest[i] = a;
  }

}
OIL_DEFINE_IMPL (divide_u32_u64_u32_asm, divide_u32_u64_u32);









