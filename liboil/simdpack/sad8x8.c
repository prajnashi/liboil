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
    "uint32_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2, int sstr2");
OIL_DEFINE_CLASS (sad8x8_f64,
    "double *dest, int dstr, double *src1, int sstr1, double *src2, int sstr2");

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

OIL_DEFINE_IMPL_REF(sad8x8_f64_ref, sad8x8_f64_class);

static void
sad8x8_s16_ref(uint32_t *dest, int16_t *src1, int sstr1, int16_t *src2,
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

OIL_DEFINE_IMPL_REF(sad8x8_s16_ref, sad8x8_s16_class);

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

/* IMPL sad8x8_s16_l15_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
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


#ifdef TEST_sad8x8_s16
int TEST_sad8x8_s16(void)
{
	int i;
	int pass;
	int failures = 0;
	int16_t *src1, *src2;
	uint32_t dest_ref, dest_test;
	struct sl_profile_struct t;

#ifdef ALIGNED16
	src1 = sl_malloc_s16_a16(64);
	src2 = sl_malloc_s16_a16(64);
#else
	src1 = sl_malloc_s16(64);
	src2 = sl_malloc_s16(64);
#endif
	
	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(sad8x8_s16_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++){
#ifdef LIMITED15
			src1[i] = sl_rand_s16_l15();
			src2[i] = sl_rand_s16_l15();
#else
			src1[i] = sl_rand_s16();
			src2[i] = sl_rand_s16();
#endif
		}

		sad8x8_s16_ref(&dest_test, src1, src2, 8*2, 8*2);
		sl_profile_start(t);
		sad8x8_s16_FUNC(&dest_ref, src1, src2, 8*2, 8*2);
		sl_profile_stop(t);

		if(dest_test != dest_ref){
			failures++;
		}
	}

	sl_free(src1);
	sl_free(src2);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

