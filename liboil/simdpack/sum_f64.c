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


static void
sum_f64_ref (double *dest, double *src, int sstr, int n)
{
	double sum = 0;
	double errsum = 0;
	double tmp;
	int i;

	for(i=0;i<n;i++){
		tmp = sum;
		sum += *OIL_OFFSET(src, sstr*i);
		errsum += (tmp - sum) + *OIL_OFFSET(src, sstr*i);
	}

	*dest = sum + errsum;
}
OIL_DEFINE_IMPL_REF (sum_f64_ref, sum_f64_class);

static void
sum_f64_i10_simple (double *dest, double *src, int sstr, int n)
{
	double sum = 0;
	int i;

	for(i=0;i<n;i++){
		sum += *OIL_OFFSET(src, sstr*i);
	}

	*dest = sum;
}
OIL_DEFINE_IMPL_REF (sum_f64_i10_simple, sum_f64_class);

static void
sum_f64_i10_unroll4 (double *dest, double *src, int sstr, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	int i;

	while (n&3) {
		sum1 += *src;
		OIL_INCREMENT (src, sstr);
		n--;
	}
	for(i=0;i<n;i+=4){
		sum1 += *OIL_OFFSET(src, sstr*i);
		sum2 += *OIL_OFFSET(src, sstr*(i+1));
		sum3 += *OIL_OFFSET(src, sstr*(i+2));
		sum4 += *OIL_OFFSET(src, sstr*(i+3));
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL_REF (sum_f64_i10_unroll4, sum_f64_class);


#ifdef TEST_sum_f64
int TEST_sum_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	double *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(1);
	dest_test = sl_malloc_f64(1);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(sum_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		int ok;

		for(i=0;i<N;i++)src[i]=sl_rand_f64_0_1();

		sum_f64_ref(dest_ref,src,N);
		sl_profile_start(t);
		sum_f64_FUNC(dest_test,src,N);
		sl_profile_stop(t);

#ifdef INACCURATE10
		ok = sl_equal_f64_i10(dest_test[0],dest_ref[0]);
#else
		ok = sl_equal_f64(dest_test[0],dest_ref[0]);
#endif
		if(!ok){
			printf("%d %g %g %g %g\n",i,src[0],dest_ref[0],
				dest_test[0],
				(dest_test[0]-dest_ref[0])/(TWO_TO_NEG52*dest_ref[0]));
		}
	}

	sl_free(src);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

