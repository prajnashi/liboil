/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(copy_u8);

static void
copy_u8_mmx (uint8_t *dest, uint8_t *src, int n)
{
  if (n&4) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  while(n&0x7) {
    *dest++ = *src++;
    n--;
  }
  asm volatile (
      "  mov $0, %%eax\n"
      "1:\n"
      "  movq (%1,%%eax), %%mm0\n"
      "  movq %%mm0, (%0,%%eax)\n"
      "  add $8, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n/8)
      : "eax");
}
OIL_DEFINE_IMPL_FULL (copy_u8_mmx, copy_u8, OIL_IMPL_REQUIRES_MMX);

static void
copy_u8_mmx2 (uint8_t *dest, uint8_t *src, int n)
{
  while (n&0xc) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  while(n&0xf) {
    *dest++ = *src++;
    n--;
  }
  asm volatile (
      "  mov $0, %%eax\n"
      "1:\n"
      "  movq (%1,%%eax), %%mm0\n"
      "  movq %%mm0, (%0,%%eax)\n"
      "  movq 8(%1,%%eax), %%mm0\n"
      "  movq %%mm0, 8(%0,%%eax)\n"
      "  add $16, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n/16)
      : "eax");
}
OIL_DEFINE_IMPL_FULL (copy_u8_mmx2, copy_u8, OIL_IMPL_REQUIRES_MMX);









