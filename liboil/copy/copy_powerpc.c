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
copy_u8_altivec (uint8_t *dest, uint8_t *src, int n)
{
  while((unsigned long)dest & 0xf) {
    *dest++ = *src++;
    n--;
  }

  if (n/16) {
    asm volatile (
        "  mtctr %2\n"
        "  li %%r11, 0\n"
        "  lvsl %%v2, 0, %1\n"
        "  lvx %%v0, 0, %1\n"
        "  addi %1, %1, 16\n"
        "1:\n"
        "  lvxl %%v1, %%r11, %1\n"
        "  vperm %%v0, %%v0, %%v1, %%v2\n"
        "  stvxl %%v0, %%r11, %0\n"
        "  vor %%v0, %%v1, %%v1\n"
        "  addi %%r11, %%r11, 16\n"
        "  bdnz+ 1b\n"
        "  add %0, %0, %%r11\n"
        "  add %1, %1, %%r11\n"
        "  addi %1, %1, -16\n"
      : "+b" (dest), "+b" (src)
      : "r" (n/16));
  }

  n &= 0xf;
  while(n) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_altivec, copy_u8, OIL_IMPL_REQUIRES_ALTIVEC);





static void
copy_u8_altivec2 (uint8_t *dest, uint8_t *src, int n)
{
  while((unsigned long)dest & 0xf) {
    *dest++ = *src++;
    n--;
  }

  if (n/16) {
    asm volatile (
        "  mtctr %2\n"
        "  li %%r11, 0\n"
        "  lvsl %%v2, 0, %1\n"
        "  lvx %%v0, 0, %1\n"
        "  addi %1, %1, 16\n"
        "1:\n"
#if 0
        "  lvxl %%v1, %%r11, %1\n"
        "  vperm %%v0, %%v0, %%v1, %%v2\n"
        "  stvxl %%v0, %%r11, %0\n"
        "  vor %%v0, %%v1, %%v1\n"
        "  addi %%r11, %%r11, 16\n"
#endif
        "  stvx %%v0, %%r11, %0\n"
        "  stvx %%v0, %%r11, %0\n"
        "  stvx %%v0, %%r11, %0\n"
        "  stvx %%v0, %%r11, %0\n"
        "  bdnz+ 1b\n"
        "  add %0, %0, %%r11\n"
        "  add %1, %1, %%r11\n"
        "  addi %1, %1, -16\n"
      : "+b" (dest), "+b" (src)
      : "r" (n/16));
  }

  n &= 0xf;
  while(n) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_altivec2, copy_u8, OIL_IMPL_REQUIRES_ALTIVEC);




static void
copy_u8_altivec3 (uint8_t *dest, uint8_t *src, int n)
{
  while((unsigned long)dest & 0xf) {
    *dest++ = *src++;
    n--;
  }

  if (n/32) {
    asm volatile (
        "  mtctr %2\n"
        "  li %%r11, 0\n"
        "  lvsl %%v3, 0, %1\n"
        "  lvx %%v0, 0, %1\n"
        "  addi %1, %1, 16\n"
        "1:\n"
        "  lvxl %%v1, %%r11, %1\n"
        "   lvxl %%v2, %%r11, %1\n"
        "    lvxl %%v3, %%r11, %1\n"
        "     lvxl %%v4, %%r11, %1\n"
        "  stvxl %%v1, %%r11, %0\n"
        "   stvxl %%v2, %%r11, %0\n"
        "    stvxl %%v3, %%r11, %0\n"
        "     stvxl %%v4, %%r11, %0\n"
        "  bdnz+ 1b\n"
        "  add %0, %0, %%r11\n"
        "  add %1, %1, %%r11\n"
        "  addi %1, %1, -16\n"
      : "+b" (dest), "+b" (src)
      : "r" (n/32));
  }

  n &= 0xf;
  while(n) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_altivec3, copy_u8, OIL_IMPL_REQUIRES_ALTIVEC);






