
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


static void mix_u8_a16_altivec(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	static const uint8_t permute[] __attribute__ ((__aligned__ (16))) = {
		0, 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30 };
	const uint8_t *perm = permute;
	int i;

	for(i=(n&~0xf);i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
	__asm__ __volatile__("\n"
		"	li	%%r12, 0		\n"
		"	vxor	%%v3, %%v3, %%v3	\n"
		"	lvx	%%v12, %4, %%r12	\n"
		"	mtctr	%5			\n"
		"1:	lvx	%%v0, %1, %%r12		\n"
		"	lvx	%%v1, %2, %%r12		\n"
		"	lvx	%%v2, %3, %%r12		\n"

		"	vmrghb	%%v4, %%v3, %%v0	\n"
		"	vmrghb	%%v5, %%v3, %%v1	\n"
		"	vmrghb	%%v6, %%v3, %%v2	\n"
		"	vmrghb	%%v7, %%v1, %%v3	\n"
		"	vsubuhs	%%v8, %%v7, %%v5	\n"
		"	vsubshs	%%v9, %%v4, %%v5	\n"
		"	vmladduhm	%%v10, %%v9, %%v6, %%v8	\n"

		"	vmrglb	%%v4, %%v3, %%v0	\n"
		"	vmrglb	%%v5, %%v3, %%v1	\n"
		"	vmrglb	%%v6, %%v3, %%v2	\n"
		"	vmrglb	%%v7, %%v1, %%v3	\n"
		"	vsubuhs	%%v8, %%v7, %%v5	\n"
		"	vsubshs	%%v9, %%v4, %%v5	\n"
		"	vmladduhm	%%v11, %%v9, %%v6, %%v8	\n"

		"	vperm	%%v0, %%v10, %%v11, %%v12	\n"
		"	stvx	%%v0, %0, %%r12		\n"
		"	addi	%%r12, %%r12, 16	\n"
		"	bdnz	1b			\n"
	: "+b" (dest), "+b" (src1), "+b" (src2), "+b" (scale), "+b" (perm)
	: "r" (n/16)
	: "12", "ctr");
}
OIL_DEFINE_IMPL (mix_u8_a16_altivec, mix_u8);


