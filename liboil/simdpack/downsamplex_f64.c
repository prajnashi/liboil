/* downsamplex f64 functions
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
Kernel: downsamplex_f64
Description: downsamples a scan line using linear interpolation

This is an excessively mathematical way of describing linear
interpolation, but it's precise:

  Let f(x) be the function where f(x) = src[floor(x)].  Let
  ratio = nsrc/ndest.  Then the output
  dest[i] = Integrate[f(x),{x,i*ratio,(i+1)*ratio}]/ratio.

Downsampling is only valid when 1.0 < ratio < 2.0.  Note that this
is "<" and not "<=".  If you want to downsample further, you should
either downsample to a power of 2, and then use downsample2, or use
downsample2 and then downsample.  The former will probably give
better results, but slower.

This function will probably be changed, as usage patterns develop.
*/

#ifndef _downsamplex_f64_h_
#define _downsamplex_f64_h_

#include <math.h>
#include <sl_types.h>


/* storage class */
#ifndef SL_downsamplex_f64_storage
 #ifdef SL_storage
  #define SL_downsamplex_f64_storage SL_storage
 #else
  #define SL_downsamplex_f64_storage static inline
 #endif
#endif


/* IMPL downsamplex_f64_ref */
SL_downsamplex_f64_storage
void downsamplex_f64_ref(f64 *dest, f64 *src, int ndest, int nsrc, f64 ratio,
	f64 invratio)
{
	int i,j;
	double x;

#if 0
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (j+1 - i*ratio) * src[j];
		if(j+2 < floor((i+1)*ratio)){
			x += src[j+1];
			x += ((i+1)*ratio - (j+2)) * src[j+2];
		}else{
			x += ((i+1)*ratio - (j+1)) * src[j+1];
		}
		dest[i] = x * invratio;
	}
#endif
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (i - j*invratio)*src[j];
		x += ((j+1)*invratio - i)*src[j+1];
		dest[i] = x * ratio;
	}
}

/* IMPL downsamplex_f64_fast */
SL_downsamplex_f64_storage
void downsamplex_f64_fast(f64 *dest, f64 *src, int ndest, int nsrc, f64 ratio,
	f64 invratio)
{
	int i,j;
	double x;
	unsigned int step;
	unsigned int acc;

	acc = 0;
	j = 0;
	step = (ratio-1.0)*(1<<16);
	for(i=0;i<ndest;i++){
//		printf("# %d %d %d %g\n",i,j,acc,floor(i*ratio));
		x = (j+1 - i*ratio) * src[j];
		x += ((i+1)*ratio - (j+1)) * src[j+1];
		dest[i] = x * invratio;
		acc += step;
		j += 1 + (acc>>16);
		acc &= (1<<16)-1;
	}
}

#endif

#ifdef TEST_downsamplex_f64
int TEST_downsamplex_f64(void)
{
	int i;
	//int pass;
	int failures = 0;
	f64 *src;
	f64 *dest_test, *dest_ref;
	struct sl_profile_struct t;
	int nsrc;
	f64 ratio, invratio;

	src = sl_malloc_f64(N*2);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20020303);

	printf("I: " sl_stringify(downsamplex_f64_FUNC) "\n");

//	for(pass=0;pass<N_PASS;pass++){
{
		for(i=0;i<N*2;i++){
			//src[i] = sl_rand_f64_0_1();
			src[i] = 100+i;
		}
		nsrc = floor(N*(1.0+sl_rand_f64_0_1()));
		ratio = ((double)nsrc)/N;
		invratio = 1.0/ratio;

		downsamplex_f64_ref(dest_ref,src,N,nsrc,ratio,invratio);
		sl_profile_start(t);
		downsamplex_f64_FUNC(dest_test,src,N,nsrc,ratio,invratio);	
		sl_profile_stop(t);

		printf("nsrc=%d\n",nsrc);
		for(i=0;i<N;i++){
			//if(!sl_equal_f64(dest_test[i],dest_ref[i])){
			dest_test[i]=100+ratio*i;
			if(!sl_equal_f64(dest_test[i],dest_ref[i])){
				printf("%d %g %g %g %g\n",i,src[i],dest_ref[i],
					dest_test[i],
					dest_test[i]-dest_ref[i]);
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


