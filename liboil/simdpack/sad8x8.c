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


OIL_DEFINE_CLASS (sad8x8_s16,
    "uint32_t *d_8x8, int ds, int16_t *s1_8x8, int ss1, int16_t *s2_8x8, int ss2");
OIL_DEFINE_CLASS (sad8x8_f64,
    "double *d_8x8, int ds, double *s1_8x8, int ss1, double *s2_8x8, int ss2");

static void
sad8x8_f64_ref(double *dest, int dstr, double *src1, int sstr1, double *src2,
    int sstr2)
{
	int i,j;
	double sum;

	sum = 0;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			sum += fabs(OIL_GET(src1,sstr1*i+j, double) -
			    OIL_GET(src2,sstr2*i+j, double));
		}
	}
	*dest = sum;
}

OIL_DEFINE_IMPL_REF(sad8x8_f64_ref, sad8x8_f64);

static void
sad8x8_s16_ref(uint32_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2,
    int sstr2)
{
	int i,j;
	int d;
	uint32_t sum;

	sum = 0;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			d = ((int)OIL_GET(src1,sstr1*i+j, int16_t)) -
				((int)OIL_GET(src2,sstr2*i+j, int16_t));
			sum += (d<0) ? -d : d;
		}
	}
	*dest = sum;
}

OIL_DEFINE_IMPL_REF(sad8x8_s16_ref, sad8x8_s16);

