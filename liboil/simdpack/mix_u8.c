
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

OIL_DEFINE_CLASS (mix_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");

static void mix_u8_ref(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
}

OIL_DEFINE_IMPL_REF (mix_u8_ref, mix_u8);

static void mix_u8_fast(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast, mix_u8);

static void
mix_u8_fast2 (uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = ((*src1 - *src2) * *scale + *src2 * 255)>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast2, mix_u8);

/* PowerPC likes this one */
static void
mix_u8_fast3(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = ((src1[i] - src2[i]) * scale[i] + src2[i] * 255)>>8;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast3, mix_u8);

static void
mix_u8_unroll4(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n&0x3){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n-=4;
	}
}
OIL_DEFINE_IMPL (mix_u8_unroll4, mix_u8);

