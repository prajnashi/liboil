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

#if 0
#ifdef HAVE_CPU_POWERPC
static void
sad8x8_s16_a16_altivec (uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}
#endif

#if 0
SL_sad8x8_s16_storage
void sad8x8_s16_l15_a16_altivec(uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvsumsws 22, 22, 19\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}
#endif
#endif


