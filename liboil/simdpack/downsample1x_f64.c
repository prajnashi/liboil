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

OIL_DEFINE_CLASS_X (downsample1x_f64,
    "double *dest, int dstr, double *src, int sstr, "
    "int ndest, int nsrc, double ratio, double invratio");

/*
Kernel: downsample1x_f64
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

static void
downsample1x_f64_ref (double *dest, int dstr, double *src, int sstr,
    int ndest, int nsrc, double ratio, double invratio)
{
	int i,j;
	//double x;

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
		j = floor((0.49999995+i)*ratio);
		dest[i] = src[j];
	}
#if 0
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (i - j*invratio)*src[j];
		x += ((j+1)*invratio - i)*src[j+1];
		dest[i] = x * ratio;
	}
#endif
}

OIL_DEFINE_IMPL_REF (downsample1x_f64_ref, downsample1x_f64_class);

static void
downsample1x_f64_fast (double *dest, int dstr, double *src, int sstr,
    int ndest, int nsrc, double ratio, double invratio)
{
	int i,j;
	double x;
#if 0
	unsigned int step;
	unsigned int acc;

	j = 0;
	step = (ratio-1.0)*((1<<31)+1);
	//acc = (1<<30) - 1;
	acc = step>>1;
	for(i=0;i<ndest;i++){
//		printf("# %d %d %d %g\n",i,j,acc,floor(i*ratio));
		dest[i] = src[j];
		acc += step;
		j += 1 + (acc>>31);
		acc &= (1<<31)-1;
	}
#else
	unsigned int step;
	unsigned int acc;
	int k;
	const unsigned int fac = 1<<16;

	x = 0.5 * ratio;
	acc = fac * x;
	step = fac * (ratio - 1.0);
	k = 0;
	for(i=0;i<ndest;i++){
		j = floor(x);
		//dest[i] = src[j];
		dest[i] = src[k];

		x += ratio;
		acc += step;
		k++;
		if(acc>fac){
			k++;
			acc -= fac;
		}
		//while(x>=1){ x-=1.0; k++; }
	}
#endif
}

OIL_DEFINE_IMPL (downsample1x_f64_fast, downsample1x_f64_class);

#ifdef TEST_downsample1x_f64
int TEST_downsample1x_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	double *src;
	double *dest_test, *dest_ref;
	struct sl_profile_struct t;
	int nsrc;
	double ratio, invratio;
	double sad, ad;

	src = sl_malloc_f64(N*2);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20020303);

	printf("I: " sl_stringify(downsample1x_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N*2;i++){
//#ifdef test_linear
#if 1
			src[i] = 100+0.5+i;
#else
			src[i] = sl_rand_f64_0_1();
#endif
		}
		nsrc = floor(N*(1.0+sl_rand_f64_0_1()));
		ratio = ((double)nsrc)/N;
		invratio = 1.0/ratio;

		downsample1x_f64_ref(dest_ref,src,N,nsrc,ratio,invratio);
		sl_profile_start(t);
		downsample1x_f64_FUNC(dest_test,src,N,nsrc,ratio,invratio);	
		sl_profile_stop(t);

		sad=0;
		ad=0;
		printf("nsrc=%d\n",nsrc);
		for(i=0;i<N;i++){
#ifdef test_linear
			dest_test[i]=100+ratio*(i+0.5);
#endif
			ad += dest_test[i] - dest_ref[i];
			sad += fabs(dest_test[i] - dest_ref[i]);
			if(!sl_equal_f64(dest_test[i],dest_ref[i])){
#if 1
				printf("%d %g %g %g %g\n",i,src[i],dest_ref[i],
					dest_test[i],
					dest_test[i]-dest_ref[i]);
#endif
				failures++;
			}
		}
		printf("sad=%g ad=%g\n",sad,ad);
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


