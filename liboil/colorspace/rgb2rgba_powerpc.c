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

OIL_DEFINE_IMPL_ASM (rgb2rgba_ppc, rgb2rgba);
