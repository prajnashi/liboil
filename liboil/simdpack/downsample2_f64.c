/* downsample2 f64 functions
 * Copyright (C) 2001,2002  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
Kernel: downsample2_f64
Description: downsamples a scan line by a factor of 2 by averaging

Averages sucessive input samples to produce output samples.

*/

#ifndef _downsample2_f64_h_
#define _downsample2_f64_h_

#include <math.h>
#include <sl_types.h>


/* storage class */
#ifndef SL_downsample2_f64_storage
 #ifdef SL_storage
  #define SL_downsample2_f64_storage SL_storage
 #else
  #define SL_downsample2_f64_storage static inline
 #endif
#endif


/* IMPL downsample2_f64_ref */
SL_downsample2_f64_storage
void downsample2_f64_ref(f64 *dest, f64 *src, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		dest[i] = (src[i*2] + src[i*2+1])/2;
	}
}

/* IMPL downsample2_f64_fast */
SL_downsample2_f64_storage
void downsample2_f64_fast(f64 *dest, f64 *src, int n)
{
	while(n>1){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=2;
	}
}

/* IMPL downsample2_f64_unroll4 */
SL_downsample2_f64_storage
void downsample2_f64_unroll4(f64 *dest, f64 *src, int n)
{
	while(n&0x6){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=2;
	}
	while(n>1){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=8;
	}
}

#endif

#ifdef TEST_downsample2_f64
int TEST_downsample2_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src;
	f64 *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(N/2);
	dest_test = sl_malloc_f64(N/2);

	sl_profile_init(t);
	srand(20021003);

	printf("I: " sl_stringify(downsample2_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src[i] = sl_rand_f64_0_1();
		}

		downsample2_f64_ref(dest_ref,src,N);
		sl_profile_start(t);
		downsample2_f64_FUNC(dest_test,src,N);	
		sl_profile_stop(t);

		for(i=0;i<N/2;i++){
			if(!sl_equal_f64(dest_test[i],dest_ref[i])){
				printf("%d %g %g %g\n",i,src[i],dest_ref[i],
					dest_test[i]);
				failures++;
			}
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


