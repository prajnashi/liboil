/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
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
#include <liboil/simdpack/simdpack.h>

#define ABS(x) ((x)>0 ? (x) : -(x))

static void
abs_u16_s16_a16_altivec (uint16_t * dest, int dstr, int16_t * src, int sstr,
    int n)
{
  int i;

  for (i = n & ~0x7; i < n; i++) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	li %%r10, 0			\n"
      "	vxor %%v2, %%v2, %%v2		\n"
      "	mtctr %2			\n"
      "1:	lvx %%v0,%%r10,%1		\n"
      "	vsubshs %%v1, %%v2, %%v0	\n"
      "	vmaxsh %%v1, %%v1, %%v0		\n"
      "	stvx %%v1,%%r10,%0		\n"
      "	addi %%r10, %%r10, 16		\n"
      "	bdnz 1b				\n":"+b" (dest), "+b" (src), "+b" (n)
      ::"10", "ctr");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_a16_altivec, abs_u16_s16, OIL_IMPL_REQUIRES_ALTIVEC);

