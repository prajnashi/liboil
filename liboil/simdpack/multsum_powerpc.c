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
#include <math.h>


/* This could probably be improved by unrolling */
static void multsum_f32_ppcasm(float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
	asm __volatile__("\n"
		"	lfs f0, 0(%0)\n"
		"	lfs f1, 0(%1)\n"
		"	fmuls f2, f0, f1\n"
		"	addi %2, %2, -1\n"
		"	mtctr %2\n"
		"1:	lfsu f0, 4(%0)\n"
		"	lfsu f1, 4(%1)\n"
		"	fmadds f2,f0,f1,f2\n"
		"	bdnz 1b\n"
		"	stfs f2, 0(%3)\n"
	: "+b" (src1), "+b" (src2), "+b" (n)
	: "b" (dest)
	: "32", "33", "34", "ctr");
}
OIL_DEFINE_IMPL_ASM (multsum_f32_ppcasm, multsum_f32);



