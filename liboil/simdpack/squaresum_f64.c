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


OIL_DEFINE_CLASS (squaresum_f64, "double *dest, double *src, int n");

static void
squaresum_f64_ref(double *dest, double *src, int n)
{
	double sum2 = 0;
	double errsum = 0;
	double tmp;
	double x;
	int i;

	for(i=0;i<n;i++){
		tmp = sum2;
		x = src[i]*src[i];
		sum2 += x;
		errsum += (tmp - sum2) + x;
	}

	*dest = sum2 + errsum;
}
OIL_DEFINE_IMPL_REF (squaresum_f64_ref, squaresum_f64_class);

static void
squaresum_f64_i10_simple(double *dest, double *src, int n)
{
	double sum2 = 0;
	int i;

	for(i=0;i<n;i++){
		sum2 += src[i]*src[i];
	}

	*dest = sum2;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_simple, squaresum_f64_class);

#if 0
#include <multsum_f64.h>
static void
squaresum_f64_i10_multsum(double *dest, double *src, int n)
{
	multsum_f64(dest,src,src,n);
}
#endif

static void
squaresum_f64_i10_unroll4a(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;

	while(n&0x3){
		sum1 += *src * *src;
		src++;
		n--;
	}
	while(n>0){
		sum1 += *src * *src;
		src++;
		sum2 += *src * *src;
		src++;
		sum3 += *src * *src;
		src++;
		sum4 += *src * *src;
		src++;
		n-=4;
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll4a, squaresum_f64_class);

static void
squaresum_f64_i10_unroll4(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	int i;

	while(n&0x3){
		sum1 += src[0]*src[0];
		src++;
		n--;
	}
	for(i=0;i<n;i+=4){
		sum1 += src[i]*src[i];
		sum2 += src[i+1]*src[i+1];
		sum3 += src[i+2]*src[i+2];
		sum4 += src[i+3]*src[i+3];
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll4, squaresum_f64_class);

static void
squaresum_f64_i10_unroll8(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	double sum5 = 0;
	double sum6 = 0;
	double sum7 = 0;
	double sum8 = 0;
	int i;

	while(n&0x7){
		sum1 += src[0]*src[0];
		src++;
		n--;
	}
	for(i=0;i<n;i+=8){
		sum1 += src[i]*src[i];
		sum2 += src[i+1]*src[i+1];
		sum3 += src[i+2]*src[i+2];
		sum4 += src[i+3]*src[i+3];
		sum5 += src[i+4]*src[i+4];
		sum6 += src[i+5]*src[i+5];
		sum7 += src[i+6]*src[i+6];
		sum8 += src[i+7]*src[i+7];
	}

	*dest = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll8, squaresum_f64_class);

#ifdef TEST_squaresum_f64
int TEST_squaresum_f64(void)
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

	printf("I: " sl_stringify(squaresum_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		int ok;

		for(i=0;i<N;i++)src[i]=sl_rand_f64_0_1();

		squaresum_f64_ref(dest_ref,src,N);
		sl_profile_start(t);
		squaresum_f64_FUNC(dest_test,src,N);
		sl_profile_stop(t);

#ifdef INACCURATE10
		ok = sl_equal_f64_i10(dest_test[0], dest_ref[0]);
#else
		ok = sl_equal_f64(dest_test[0], dest_ref[0]);
#endif
		if(!ok){
			printf("%d %g %g %g\n",pass,src[0],dest_ref[0], dest_test[0]);
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

