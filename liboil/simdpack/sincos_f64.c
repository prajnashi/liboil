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
sincos_f64_ref (double *dest_sin, double *dest_cos, int n, double offset,
    double interval)
{
	int i;

	for(i=0;i<n;i++){
		dest_sin[i] = sin(offset + interval*i);
		dest_cos[i] = cos(offset + interval*i);
	}
}
OIL_DEFINE_IMPL_REF (sincos_f64_ref, sincos_f64_class);

/* XXX This gets more inaccurate as N increases */
static void
sincos_f64_i20_fast(double *dest_sin, double *dest_cos, int n, double offset,
    double interval)
{
	int i;
	double c,s,dc,ds,temp_s,temp_c;

	dest_sin[0] = s = sin(offset);
	dest_cos[0] = c = cos(offset);
	ds = sin(interval);
	dc = cos(interval);
	for(i=1;i<n;i++){
		temp_s = s*dc + c*ds;
		temp_c = c*dc - s*ds;
		dest_sin[i] = s = temp_s;
		dest_cos[i] = c = temp_c;
	}
}
OIL_DEFINE_IMPL_REF (sincos_f64_i20_fast, sincos_f64_class);


#ifdef TEST_sincos_f64
int TEST_sincos_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *dest_ref, *dest_test;
	struct sl_profile_struct t;

	dest_ref = sl_malloc_f64(N*2);
	dest_test = sl_malloc_f64(N*2);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(sincos_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		double interval;
		double offset;
		
		interval = sl_rand_f64_0_1();
		offset = sl_rand_f64_0_1();

		sincos_f64_ref(dest_ref,dest_ref+N,N,offset,interval);
		sl_profile_start(t);
		sincos_f64_FUNC(dest_test,dest_test+N,N,offset,interval);
		sl_profile_stop(t);

		for(i=0;i<N*2;i++){
			int ok;
#ifdef INACCURATE20
			ok = fabs(dest_test[i]-dest_ref[i])<100*TWO_TO_NEG52;
#else
			ok = fabs(dest_test[i]-dest_ref[i])<TWO_TO_NEG52;
#endif
			//ok = sl_equal_f64_i30(dest_test[i],dest_ref[i]);
			if(!ok){
				printf("%d %g %g\n",i,dest_ref[i],dest_test[i]);
				failures++;
			}
		}
	}

	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

