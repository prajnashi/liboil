/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2004  David A. Schleef <ds@schleef.org>
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









