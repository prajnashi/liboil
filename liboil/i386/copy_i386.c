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
  if (n) asm volatile (
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
OIL_DEFINE_IMPL_FULL (copy_u8_mmx, copy_u8, OIL_IMPL_FLAG_MMX);

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
  if (n) asm volatile (
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
OIL_DEFINE_IMPL_FULL (copy_u8_mmx2, copy_u8, OIL_IMPL_FLAG_MMX);

static void
copy_u8_mmx3 (uint8_t *dest, uint8_t *src, int n)
{
  /* make sure destination is cache-line aligned for output */
  if (n < 64) {
    while (n>0) {
      *dest++ = *src++;
      n--;
    }
    return;
  }
  while (((unsigned long)dest) & 0x3) {
    *dest++ = *src++;
    n--;
  }
  while (((unsigned long)dest) & 0x3f) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  if (n > 64) asm volatile (
      "  mov $0, %%eax\n"
      "1:\n"
      //"  prefetchnta 128(%1,%%eax)\n"
      "  movq (%1,%%eax), %%mm0\n"
      "  movq 8(%1,%%eax), %%mm1\n"
      "  movq 16(%1,%%eax), %%mm2\n"
      "  movq 24(%1,%%eax), %%mm3\n"
      "  movq 32(%1,%%eax), %%mm4\n"
      "  movq 40(%1,%%eax), %%mm5\n"
      "  movq 48(%1,%%eax), %%mm6\n"
      "  movq 56(%1,%%eax), %%mm7\n"
      "  movntq %%mm0, (%0,%%eax)\n"
      "  movntq %%mm1, 8(%0,%%eax)\n"
      "  movntq %%mm2, 16(%0,%%eax)\n"
      "  movntq %%mm3, 24(%0,%%eax)\n"
      "  movntq %%mm4, 32(%0,%%eax)\n"
      "  movntq %%mm5, 40(%0,%%eax)\n"
      "  movntq %%mm6, 48(%0,%%eax)\n"
      "  movntq %%mm7, 56(%0,%%eax)\n"
      "  add $64, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  sfence\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n>>6)
      : "eax");

  dest += n&(~(0x3f));
  src += n&(~(0x3f));
  n &= 0x3f;
  while (n > 3) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  while (n > 0) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_mmx3, copy_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
copy_u8_mmx4 (uint8_t *dest, uint8_t *src, int n)
{
  /* make sure destination is cache-line aligned for output */
  if (n < 32) {
    while (n>0) {
      *dest++ = *src++;
      n--;
    }
    return;
  }
  while (((unsigned long)dest) & 0x3) {
    *dest++ = *src++;
    n--;
  }
  while (((unsigned long)dest) & 0x1f) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  if (n > 32) asm volatile (
      "  mov $0, %%eax\n"
      "1:\n"
      //"  prefetchnta 128(%1,%%eax)\n"
      "  movq (%1,%%eax), %%mm0\n"
      "  movq 8(%1,%%eax), %%mm1\n"
      "  movq 16(%1,%%eax), %%mm2\n"
      "  movq 24(%1,%%eax), %%mm3\n"
      "  movntq %%mm0, (%0,%%eax)\n"
      "  movntq %%mm1, 8(%0,%%eax)\n"
      "  movntq %%mm2, 16(%0,%%eax)\n"
      "  movntq %%mm3, 24(%0,%%eax)\n"
      "  add $32, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  sfence\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n>>5)
      : "eax");

  dest += n&(~(0x1f));
  src += n&(~(0x1f));
  n &= 0x1f;
  while (n > 3) {
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
    n-=4;
  }
  while (n > 0) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_mmx4, copy_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
copy_u8_mmx5 (uint8_t *dest, uint8_t *src, int n)
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
  if (n) asm volatile (
      "  mov $0, %%eax\n"
      "1:\n"
      "  movq (%1,%%eax), %%mm0\n"
      "  movq 8(%1,%%eax), %%mm1\n"
      "  movq %%mm0, (%0,%%eax)\n"
      "  movq %%mm1, 8(%0,%%eax)\n"
      "  add $16, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n/16)
      : "eax");
}
OIL_DEFINE_IMPL_FULL (copy_u8_mmx5, copy_u8, OIL_IMPL_FLAG_MMX);

