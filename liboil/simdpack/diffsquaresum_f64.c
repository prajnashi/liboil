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

OIL_DEFINE_CLASS (diffsquaresum_f64,
    "double *dest, double *src1, int sstr1, double *src2, int sstr2, int n");

static void
diffsquaresum_f64_ref(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum = 0;
	double errsum = 0;
	double tmp;
	double x;
	int i;

	for(i=0;i<n;i++){
		x = OIL_OFFSET(src1, i*sstr1) - OIL_OFFSET(src2, i*sstr2);
		x = x*x;
		tmp = sum;
		sum += x;
		errsum += (tmp - sum) + x;
	}

	*dest = sum + errsum;
}

OIL_DEFINE_IMPL_REF (diffsquaresum_f64_ref, diffsquaresum_f64);

static void
diffsquaresum_f64_i10_simple(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum = 0;
	double x;
	int i;

	for(i=0;i<n;i++){
		x = OIL_OFFSET(src1, i*sstr1) - OIL_OFFSET(src2, i*sstr2);
		x = x*x;
		sum += x;
	}

	*dest = sum;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_simple, diffsquaresum_f64);

static void
diffsquaresum_f64_i10_unroll2(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum0 = 0;
	double sum1 = 0;
	double x;

	if(n&1){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n--;
	}
	while(n>0){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum1 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n-=2;
	}

	*dest = sum0 + sum1;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_unroll2, diffsquaresum_f64);

static void diffsquaresum_f64_i10_unroll4(double *dest, double *src1, int sstr1,
	double *src2, int sstr2, int n)
{
	double sum0 = 0;
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double x;

	while(n&3){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n--;
	}
	while(n>0){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum1 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum2 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum3 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n-=4;
	}

	*dest = sum0 + sum1 + sum2 + sum3;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_unroll4, diffsquaresum_f64);


#ifdef TEST_diffsquaresum_f64
int TEST_diffsquaresum_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	double *src1, *src2, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src1 = sl_malloc_f64(N);
	src2 = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(1);
	dest_test = sl_malloc_f64(1);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(diffsquaresum_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		int ok;

		for(i=0;i<N;i++){
			src1[i]=sl_rand_f64_0_1();
			src2[i]=sl_rand_f64_0_1();
		}

		diffsquaresum_f64_ref(dest_ref,src1,src2,N);
		sl_profile_start(t);
		diffsquaresum_f64_FUNC(dest_test,src1,src2,N);
		sl_profile_stop(t);

#ifdef INACCURATE10
		ok = sl_equal_f64_i10(dest_test[0], dest_ref[0]);
#else
		ok = sl_equal_f64(dest_test[0], dest_ref[0]);
#endif
		if(!ok){
			printf("%d %g %g %g\n",pass,src1[0],dest_ref[0], dest_test[0]);
		}
	}

	sl_free(src1);
	sl_free(src2);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

