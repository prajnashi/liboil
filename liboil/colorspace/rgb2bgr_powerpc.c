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
#include <liboil/liboilfuncs.h>

OIL_DECLARE_CLASS (rgb2bgr);

static void
rgb2bgr_ppc (uint8_t *dest, uint8_t* src, int n)
{
  if (n&3) {
#if 0
    /* FIXME */
    rgb2bgr_ref (dest, src, n&3);
#endif
    dest += 3 * (n&3);
    src += 3 * (n&3);
  }
  n /= 4;
  dest -= 4;
  src -= 4;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lwzu 10, 4(%1)		\n" /* rgbr x */
	"	rotlwi 10, 10, 16	\n" /* brrg x */
	"	and 11, 10, %3		\n" /* -r-g .x */
	"	sub 10, 10, 11		\n" /* b-r- x. */
	"	andi. 12, 11, 0xFF	\n" /* ---g ..x */
	"	rotlwi 12, 12, 16	\n" /* -g-- ..x */
	"	or 10, 10, 12		\n" /* bgr- x.. */
	"	lwzu 12, 4(%1)		\n" /* gbrg ..x */
	"	and 13, 12, %4		\n" /* -br- ...x */
	"	sub 12, 12, 13		\n" /* g--g ..x. */
	"	rotlwi 13, 13, 16	\n" /* r--b ...x */
	"	andis. 11, 11, 0xFF	\n" /* -r-- .x.. */
	"	or 12, 12, 11		\n" /* gr-g ..x. */
	"	andi. 11, 13, 0xFF	\n" /* ---b .x.. */
	"	or 10, 10, 11		\n" /* bgrb x... */
	"	stwu 10, 4(%0)		\n"
	"	lwzu 10, 4(%1)		\n" /* brgb x... */
	"	rotlwi 10, 10, 16	\n" /* gbbr x... */
	"	and 11, 10, %3		\n" /* -b-r .x.. */
	"	sub 10, 10, 11		\n" /* g-b- x... */
	"	andis. 13, 13, 0xFF00	\n" /* r--- ...x */
	"	or 11, 11, 13		\n" /* rb-r .x.. */
	"	andi. 13, 10, 0xFF00	\n" /* --b- ...x */
	"       or 12, 12, 13		\n" /* grbg ..x. */
	"	stwu 12, 4(%0)		\n"
	"	andis. 10, 10, 0xFF00	\n" /* g--- x... */
	"	rotlwi 10, 10, 16	\n" /* --g- x... */
	"	or 11, 11, 10		\n" /* rbgr .x.. */
	"	stwu 11, 4(%0)		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n), "b" (0x00FF00FF), "b" (0x00FFFF00)
      : "10", "11", "12", "13", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc, rgb2bgr);

static void
rgb2bgr_ppc2 (uint8_t *dest, uint8_t* src, int n)
{
  asm volatile (
	"	mtctr %2		\n"
	"1:	lswi 10, %1, 3		\n"
	"	addi %1, %1, 3		\n"
	"	andis. 11, 10, 0xFF	\n"
	"	rotlwi 10, 10, 16	\n"
	"	or 10, 10, 11		\n"
	"	stswi 10, %0, 3		\n"
	"	addi %0, %0, 3		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "11", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc2, rgb2bgr);

static void
rgb2bgr_ppc3 (uint8_t *dest, uint8_t* src, int n)
{
  dest--;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lbz 10, 2(%1)		\n"
	"	stbu 10, 1(%0)		\n"
	"	lbz 10, 1(%1)		\n"
	"	stbu 10, 1(%0)		\n"
	"	lbz 10, 0(%1)		\n"
	"	stbu 10, 1(%0)		\n"
	"	addi %1, %1, 3		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc3, rgb2bgr);

static void
rgb2bgr_ppc4 (uint8_t *dest, uint8_t* src, int n)
{
  dest--;
  src--;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lbzu 10, 1(%1)		\n"
	"	lbzu 11, 1(%1)		\n"
	"	lbzu 12, 1(%1)		\n"
	"	stbu 12, 1(%0)		\n"
	"	stbu 11, 1(%0)		\n"
	"	stbu 10, 1(%0)		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "11", "12", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc4, rgb2bgr);

