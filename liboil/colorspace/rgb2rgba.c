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

OIL_DEFINE_CLASS (rgb2rgba, "uint8_t *d_4xn, uint8_t* s_3xn, int n");

static void
rgb2rgba_ref (uint8_t *dest, uint8_t* src, int n)
{
  int i;
  
  for (i = 0; i < n; i++) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = 0xFF;
    dest += 4;
    src += 3;
  }
}

OIL_DEFINE_IMPL_REF (rgb2rgba_ref, rgb2rgba);

#ifdef HAVE_CPU_POWERPC
static void
rgb2rgba_ppc (uint8_t *dest, uint8_t* src, int n)
{
  n /= 3;
  dest -= 4;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lswi 10, %1, 3		\n"
	"	addi %1, %1, 3		\n"
	"	ori 10, 10, 0xFF	\n"
	"	stwu 10, 4(%0)		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "ctr");
}

OIL_DEFINE_IMPL (rgb2rgba_ppc, rgb2rgba);
#endif
